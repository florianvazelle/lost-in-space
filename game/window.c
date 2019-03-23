#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <nanomsg/nn.h>
#include <nanomsg/bus.h>

#include "crosshair.h"
#include "space.h"
#include "skybox.h"
#include "cockpit.h"
#include "interact.h"

#include "utils/send_info.h"
#include "utils/load_texture.h"

#define LENGTH 3

static int _wW = 800, _wH = 600;
static int _xm = 400, _ym = 300;
static GLuint _pBasicId = 0;
static GLuint _pModelId = 0;

static float xClip, yClip;
static GLfloat angleY;
static int view = 0;

static GLuint _plane = 0;
static int on_spaceship = 1;
static GLuint interact = 0;
static int find = 0;

enum kyes_t { KLEFT = 0, KRIGHT, KUP, KDOWN };

static GLuint _keys[] = {0, 0, 0, 0};

typedef struct  {
        GLfloat x, y, z;
        GLfloat theta;
} cam_t;

static cam_t _cam = {0, 0, 1.0, 0};

static GLuint _phong = 1;

extern void assimpInit(const char * filename);
extern void assimpDrawScene(void);
extern void assimpQuit(void);

static int sock;
static void initNode(int, char **);

static void initGL(void);
static void initData(void);
static void resize(int w, int h);
static void idle(void);
static void keydown(int keycode);
static void keyup(int keycode);
static void pmotion(int x, int y);
static void draw(void);
static void quit(void);

int main(int argc, char **argv) {
        if (argc < 2) {
                fprintf(stderr, "Usage: bus <NODE_ID>\n");
                exit(1);
        }
        if (!gl4duwCreateWindow(argc, argv, "GL4Dummies", 10, 10, _wW, _wH,
                                GL4DW_RESIZABLE | GL4DW_SHOWN))
                return 1;
        assimpInit("assets/models/spaceship.obj");
        initNode(argc, argv);
        initGL();
        initData();
        atexit(quit);
        gl4duwResizeFunc(resize);
        gl4duwKeyUpFunc(keyup);
        gl4duwKeyDownFunc(keydown);
        gl4duwPassiveMotionFunc(pmotion);
        gl4duwDisplayFunc(draw);
        gl4duwIdleFunc(idle);
        gl4duwMainLoop();
        return 0;
}

void fatal(const char *func) {
        fprintf(stderr, "%s: %s\n", func, nn_strerror(nn_errno()));
        exit(1);
}

void node(const int argc, const char argv[3][21]) {
        if ((sock = nn_socket (AF_SP, NN_BUS)) < 0) {
                fatal("nn_socket");
        }
        if (nn_bind(sock, argv[0]) < 0) {
                fatal("nn_bind");
        }

        sleep(1); // wait for peers to bind
        for (int x = 1; x < LENGTH; x++) {
                printf("connect %s\n", argv[x]);
                if (nn_connect(sock, argv[x]) < 0) {
                        fatal("nn_connect");
                }
        }

        sleep(1); // wait for connections
        int to = 100;
        if (nn_setsockopt(sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to,
                          sizeof (to)) < 0) {
                fatal("nn_setsockopt");
        }
}

static void initNode(int argc, char **argv){
        int nb_node = atoi(argv[1]);

        char urls[LENGTH][21];
        for(int i = 0; i < LENGTH; i++) {
                int idx = (i != nb_node) ? (i > nb_node) ? i : i + 1 : 0;
                sprintf(urls[idx], "ipc:///tmp/node%d.ipc", i);
        }

        node(LENGTH + 2, urls);
}

static void initGL() {
        glClearColor(0.0, 0.0, 0.0, 0.0);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        _pBasicId = gl4duCreateProgram("<vs>assets/shaders/basic.vs", "<gs>assets/shaders/basic.gs","<fs>assets/shaders/basic.fs", NULL);
        _pModelId = gl4duCreateProgram("<vs>assets/shaders/model.vs", "<fs>assets/shaders/model.fs", NULL);

        gl4duGenMatrix(GL_FLOAT, "viewMatrix");
        gl4duGenMatrix(GL_FLOAT, "modelMatrix");
        gl4duGenMatrix(GL_FLOAT, "projectionMatrix");

        resize(_wW, _wH);
}

static void initData(void) {
        _plane = gl4dgGenQuadf();

        init_crosshair();
        init_space();
        init_skybox();
        init_cockpit();

        init_interact();
}

static void resize(int w, int h) {
        _wW = w;
        _wH = h;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, _wW, _wH);
        gl4duBindMatrix("projectionMatrix");
        gl4duLoadIdentityf();
        gl4duFrustumf(-0.005, 0.005, -0.005 * _wH / _wW, 0.005 * _wH / _wW, 0.01, 1000.0);
}

static void idle(void) {
        xClip = ((_xm + 0.5f) / _wW) * 2.0f - 1.0f;
        yClip = 1.0f - ((_ym + 0.5f) / _wH) * 2.0f;

        GLfloat angleX = (_xm - (_wW >> 1)) / (GLfloat)_wW;
        angleY = (_ym - (_wH >> 1)) / (GLfloat)_wH;

        double dt, dtheta = M_PI, step = 150.0;
        static double t0 = 0, t;
        dt = ((t = gl4dGetElapsedTime()) - t0) / 1000.0;
        t0 = t;

        if(on_spaceship == 1) {
                sphere test_interact = { _cam.x, _cam.y, _cam.z, 10.0f };
                entitie player = {{ _cam.x, _cam.y, _cam.z, 1.5f }, { 0, 0, 0 }};

                if(angleX > _wW/2)
                        _cam.theta += dt * dtheta * tan(angleX);
                if(angleX < _wW/2)
                        _cam.theta -= dt * dtheta * tan(angleX);
                if(_keys[KUP]) {
                        player.dir.x = -dt * step * sin(_cam.theta);
                        player.dir.y = -dt * step * tan(angleY * 2);
                        player.dir.z = -dt * step * cos(_cam.theta);
                }
                if(_keys[KDOWN]) {
                        player.dir.x = dt * step * sin(_cam.theta);
                        player.dir.y = dt * step * tan(angleY * 2);
                        player.dir.z = dt * step * cos(_cam.theta);
                }

                update_space(_cam.x, _cam.y, _cam.z);
                interact = hit_interact(test_interact);
                if(hit_player_satellite(player) == 0) {
                        _cam.x = player.data.x + player.dir.x;
                        _cam.y = player.data.y + player.dir.y;
                        _cam.z = player.data.z + player.dir.z;
                }
        } else {
                if(angleX > _wW/2)
                        _cam.theta += dt * dtheta * tan(angleX);
                if(angleX < _wW/2)
                        _cam.theta -= dt * dtheta * tan(angleX);
                if(_keys[KUP]) {
                        _cam.x += -dt * step * sin(_cam.theta);
                        _cam.z += -dt * step * cos(_cam.theta);
                }
                if(_keys[KDOWN]) {
                        _cam.x += dt * step * sin(_cam.theta);
                        _cam.z += dt * step * cos(_cam.theta);
                }
        }
        //printf("x: %.02f - y: %.02f - z: %.02f\n", _cam.x, _cam.y, _cam.z);
}

static void keydown(int keycode) {
        GLint v[2];
        switch (keycode) {
        case GL4DK_LEFT:
                _keys[KLEFT] = 1;
                break;
        case GL4DK_RIGHT:
                _keys[KRIGHT] = 1;
                break;
        case GL4DK_UP:
                _keys[KUP] = 1;
                break;
        case GL4DK_DOWN:
                _keys[KDOWN] = 1;
                break;
        case GL4DK_ESCAPE:
        case 'q':
                exit(0);
        /* when 'w' pressed, toggle between line and filled mode */
        case 'w':
                glGetIntegerv(GL_POLYGON_MODE, v);
                if(v[0] == GL_FILL) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        glLineWidth(3.0);
                } else {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        glLineWidth(1.0);
                }
                break;

        case 'p':
                _phong = !_phong;
                break;
        /* Interagir */
        case GL4DK_RETURN:
                if(interact != 0) {
                        on_spaceship = !on_spaceship;
                        _cam.x = _cam.y = _cam.z = 0;
                }
                break;
        case 'f':
                find = !find;
                break;
        /* Points de vue */
        case '0':
                view = 0; // Normal
                break;
        case '1':
                view = 1; // Interieur
                break;
        }
}

static void keyup(int keycode) {
        switch (keycode) {
        case GL4DK_LEFT:
                _keys[KLEFT] = 0;
                break;
        case GL4DK_RIGHT:
                _keys[KRIGHT] = 0;
                break;
        case GL4DK_UP:
                _keys[KUP] = 0;
                break;
        case GL4DK_DOWN:
                _keys[KDOWN] = 0;
                break;
        default:
                break;
        }
}

static void pmotion(int x, int y) {
        _xm = x;
        _ym = y;
}

static void draw() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Affichage dans l'espace */
        if(on_spaceship == 1) {
                gl4duBindMatrix("viewMatrix");
                gl4duLoadIdentityf();
                /* Vu exterieur */
                if(view == 0)
                        gl4duLookAtf(_cam.x, _cam.y, _cam.z,
                                     _cam.x - sin(_cam.theta), _cam.y - angleY,  _cam.z - cos(_cam.theta),
                                     0.0, 1.0, 0.0);
                /* Vu interieur */
                else if(view == 1)
                        gl4duLookAtf(_cam.x, _cam.y + 0.18, _cam.z,
                                     _cam.x - sin(_cam.theta), _cam.y - tan(angleY * 3),  _cam.z - cos(_cam.theta),
                                     0.0, 1.0, 0.0);

                draw_skybox(_cam.x, _cam.y, _cam.z);
                draw_space(_pBasicId, _phong);

                if(view == 0) {
                        glUseProgram(_pModelId);
                        gl4duBindMatrix("modelMatrix");
                        gl4duLoadIdentityf();

                        gl4duTranslatef(_cam.x - sin(_cam.theta), _cam.y - 0.18 - angleY,  _cam.z - cos(_cam.theta));
                        gl4duRotatef((_cam.theta * 180.0 / M_PI) + 180.0, 0, 1, 0);
                        gl4duRotatef((angleY * 180.0 / M_PI), 1, 0, 0);
                        gl4duRotatef(-10, 1, 0, 0);
                        gl4duScalef(2.0 / 5.0, 2.0 / 5.0, 2.0 / 5.0);

                        gl4duRotatef(-(xClip * 180.0 / M_PI), 0, 1, 0);
                        gl4duRotatef(-(yClip * 180.0 / M_PI), 1, 0, 0);

                        apply_stars(_pModelId);
                        assimpDrawScene();

                        draw_crosshair(xClip, yClip, _pBasicId);
                } else if(view == 1) {
                        draw_cockpit(_pBasicId);
                }
        } else {
                gl4duBindMatrix("viewMatrix");
                gl4duLoadIdentityf();
                gl4duLookAtf(_cam.x, 3.0, _cam.z, _cam.x - sin(_cam.theta),
                             3.0 - (_ym - (_wH >> 1)) / (GLfloat)_wH,
                             _cam.z - cos(_cam.theta), 0.0, 1.0, 0.0);

                draw_skybox(_cam.x, 3.0, _cam.z);

                glUseProgram(_pBasicId);
                gl4duBindMatrix("modelMatrix");
                gl4duLoadIdentityf();

                glActiveTexture(GL_TEXTURE0);
                glUniform1i(glGetUniformLocation(_pBasicId, "myTexture"), 0);
                glUniform1i(glGetUniformLocation(_pBasicId, "phong"), 0);

                static float _planeScale = 100.0f;
                gl4duPushMatrix(); {
                        //gl4duTranslatef(_cam.x, 0, _cam.z);
                        gl4duRotatef(-90, 1, 0, 0);
                        gl4duScalef(_planeScale, _planeScale, 1);
                        gl4duSendMatrices();
                } gl4duPopMatrix();
                glBindTexture(GL_TEXTURE_2D, interact);
                gl4dgDraw(_plane);
                glUseProgram(0);
        }

        if(interact != 0) {
                draw_interact(_pBasicId);
        }

        if(on_spaceship == 1) {
                /**
                 * Partie Reseau
                 **/
                /* Transforme les coordonnees du joueur en message a envoye */
                vector3 coordinates = {_cam.x, _cam.y, _cam.z};
                char* message = struct2str(1, coordinates);

                /* Envoye du message */
                int sz_n = strlen(message) + 1; // '\0' too
                if (nn_send(sock, message, sz_n, NN_DONTWAIT) < 0) {
                        fatal("nn_send");
                }

                /* Reception des messages */
                char *buf = NULL;
                int recv = nn_recv(sock, &buf, NN_MSG, NN_DONTWAIT);
                if (recv >= 0) {
                        /* Traduction du message en coordonnees */
                        vector3 client = str2stuct(buf);
                        nn_freemsg(buf);

                        /* Affichage du joueur */
                        glUseProgram(_pModelId);
                        gl4duBindMatrix("modelMatrix");
                        gl4duLoadIdentityf();

                        gl4duTranslatef(client.x, client.y, client.z);
                        gl4duScalef(2.0 / 5.0, 2.0 / 5.0, 2.0 / 5.0);

                        apply_stars(_pModelId);
                        assimpDrawScene();

                        if(find == 1) {
                                //draw_pointer(client);
                        }

                        glUseProgram(0);
                }
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
}

static void quit(void) {
        assimpQuit();
        quit_crosshair();
        quit_space();
        quit_skybox();
        gl4duClean(GL4DU_ALL);
}

#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>

#include "config.h"
#include "crosshair.h"
#include "space.h"
#include "skybox.h"
#include "cockpit.h"
#include "interact.h"
#include "landed.h"
#include "p2p.h"

#include "utils/convert.h"
#include "utils/load_texture.h"

static int _wW = 800, _wH = 600;
static int _xm = 400, _ym = 300;

static float xClip, yClip;
static GLfloat angleY;
static int view = 0;

static int on_spaceship = 1;
static GLuint interact = 0;

enum kyes_t { KLEFT = 0, KRIGHT, KUP, KDOWN };

static GLuint _keys[] = {0, 0, 0, 0};

typedef struct  {
        GLfloat x, y, z;
        GLfloat theta;
} cam_t;

static cam_t _cam = {0, 0, 1.0, 0};

extern void assimpInit(const char * filename);
extern void assimpDrawScene(void);
extern void assimpQuit(void);

static int sock;

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
                printf("Usage: %s <NODE_ID>\n", argv[0]);
                return 1;
        }
        sock = initNode(argc, argv);
        if (!gl4duwCreateWindow(argc, argv, "GL4Dummies", 10, 10, _wW, _wH,
                                GL4DW_RESIZABLE | GL4DW_SHOWN))
                return 1;
        assimpInit("assets/models/spaceship.obj");
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

static void initGL() {
        glClearColor(0.0, 0.0, 0.0, 0.0);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        init_config();

        gl4duGenMatrix(GL_FLOAT, "viewMatrix");
        gl4duGenMatrix(GL_FLOAT, "modelMatrix");
        gl4duGenMatrix(GL_FLOAT, "projectionMatrix");

        resize(_wW, _wH);
}

static void initData(void) {
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

        case 'l':
                _light = !_light;
                break;
        /* Interagir */
        case GL4DK_RETURN:
                if(interact != 0) {
                        on_spaceship = !on_spaceship;
                        _cam.x = _cam.y = _cam.z = 0;
                }
                break;
        /* Points de vue */
        case '0':
                view = 0; // Exterieur
                break;
        case '1':
                view = 1; // Interieur
                break;
        case '2':
                explose_test(); //Debug
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

/**
 * Recupere la position de la souris
 **/
static void pmotion(int x, int y) {
        _xm = x;
        _ym = y;
}

/**
 * Dessiner tout
 **/
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
                draw_space(_light);

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

                        apply_stars();
                        assimpDrawScene();

                        draw_crosshair(xClip, yClip);
                } else if(view == 1) {
                        draw_cockpit();
                }
        }
        /* Affichage sur un satellite */
        else {
                gl4duBindMatrix("viewMatrix");
                gl4duLoadIdentityf();
                gl4duLookAtf(_cam.x, 3.0, _cam.z, _cam.x - sin(_cam.theta),
                             3.0 - (_ym - (_wH >> 1)) / (GLfloat)_wH,
                             _cam.z - cos(_cam.theta), 0.0, 1.0, 0.0);

                draw_skybox(_cam.x, 3.0, _cam.z);
                draw_landed(interact);
        }

        if(interact != 0) {
                draw_interact();
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

                        apply_stars();
                        assimpDrawScene();

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

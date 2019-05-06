#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>

#include "config.h"

#include "view/cockpit.h"
#include "view/crosshair.h"
#include "view/interact.h"
#include "view/skybox.h"
#include "view/credit.h"

#include "level/landed.h"
#include "level/space.h"

#include "menu/launcher.h"
#include "network/p2p.h"

#include "action/hypervelocity.h"
#include "action/laser.h"

#include "util/convert.h"
#include "util/load_texture.h"

static int _xm = 400, _ym = 300;

static float xClip, yClip;
static GLfloat angleY;
static int view = 0;

static int hypervelocity = 0;
static int on_spaceship = 1;
static GLuint interact = 0;

enum kyes_t { KLEFT = 0, KRIGHT, KUP, KDOWN };

static GLuint _keys[] = {0, 0, 0, 0};

static cam_t _cam = {0, 0, 1.0, 0};

extern void assimpInit(const char *filename);
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
static void mousepressed(int button, int state, int x, int y);
static void draw(void);
static void quit(void);

int main(int argc, char **argv) {
        if (argc < 2) {
                printf("Usage: %s <NODE_ID>\n", argv[0]);
                return 1;
        }
        sock = initNode(argc, argv);
        if (!gl4duwCreateWindow(argc, argv, "Lost in Space!", 10, 10, 800, 600,
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
        gl4duwMouseFunc(mousepressed);
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
        init_launcher();
        init_credit();

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
        gl4duFrustumf(-0.005, 0.005, -0.005 * _wH / _wW, 0.005 * _wH / _wW, 0.01,
                      1000.0);
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

        if (on_spaceship == 1) {
                sphere test_interact = {_cam.x, _cam.y, _cam.z, 10.0f};
                entitie player = {{_cam.x, _cam.y, _cam.z, 1.5f}, {0, 0, 0}};

                if (angleX > _wW / 2)
                        _cam.theta += dt * dtheta * tan(angleX);
                if (angleX < _wW / 2)
                        _cam.theta -= dt * dtheta * tan(angleX);
                if (_keys[KUP]) {
                        player.dir.x = -dt * step * sin(_cam.theta);
                        player.dir.y = -dt * step * tan(angleY * 2);
                        player.dir.z = -dt * step * cos(_cam.theta);
                }
                if (_keys[KDOWN]) {
                        player.dir.x = dt * step * sin(_cam.theta);
                        player.dir.y = dt * step * tan(angleY * 2);
                        player.dir.z = dt * step * cos(_cam.theta);
                }

                update_space(_cam.x, _cam.y, _cam.z);
                interact = hit_interact(test_interact);
                if (hit_player_satellite(player) == 0) {
                        _cam.x = player.data.x + player.dir.x;
                        _cam.y = player.data.y + player.dir.y;
                        _cam.z = player.data.z + player.dir.z;
                }
        } else {
                if (angleX > _wW / 2)
                        _cam.theta += dt * dtheta * tan(angleX);
                if (angleX < _wW / 2)
                        _cam.theta -= dt * dtheta * tan(angleX);
                if (_keys[KUP]) {
                        _cam.x += -dt * step * sin(_cam.theta);
                        _cam.z += -dt * step * cos(_cam.theta);
                }
                if (_keys[KDOWN]) {
                        _cam.x += dt * step * sin(_cam.theta);
                        _cam.z += dt * step * cos(_cam.theta);
                }
        }
        // printf("x: %.02f - y: %.02f - z: %.02f\n", _cam.x, _cam.y, _cam.z);
}

static void keydown(int keycode) {
        if (_keyboard == QWERTY) {
                switch (keycode) {
                case 'a':
                        _keys[KLEFT] = 1;
                        break;
                case 'd':
                        _keys[KRIGHT] = 1;
                        break;
                case 'w':
                        _keys[KUP] = 1;
                        break;
                case 's':
                        _keys[KDOWN] = 1;
                        break;
                }
        } else if (_keyboard == AZERTY) {
                switch (keycode) {
                case 'd':
                        _keys[KLEFT] = 1;
                        break;
                case 'q':
                        _keys[KRIGHT] = 1;
                        break;
                case 'z':
                        _keys[KUP] = 1;
                        break;
                case 's':
                        _keys[KDOWN] = 1;
                        break;
                }
        }

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
                _state = MENU;
                reinit_credit();
                break;
        case 'l':
                _light = !_light;
                break;
        /* Interagir */
        case GL4DK_RETURN:
                if (interact != 0) {
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
                explose_test(); // Debug
                break;
        }
}

static void keyup(int keycode) {
        if (_keyboard == QWERTY) {
                switch (keycode) {
                case 'a':
                        _keys[KLEFT] = 0;
                        break;
                case 'd':
                        _keys[KRIGHT] = 0;
                        break;
                case 'w':
                        _keys[KUP] = 0;
                        break;
                case 's':
                        _keys[KDOWN] = 0;
                        break;
                }
        } else if (_keyboard == AZERTY) {
                switch (keycode) {
                case 'q':
                        _keys[KLEFT] = 0;
                        break;
                case 'd':
                        _keys[KRIGHT] = 0;
                        break;
                case 'z':
                        _keys[KUP] = 0;
                        break;
                case 's':
                        _keys[KDOWN] = 0;
                        break;
                }
        }

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

void sdl2gl(float mouse[2]) {
        mouse[0] = (mouse[0] / _wW) * 2.0f - 1.0f;
        mouse[1] = 1.0f - (mouse[1] / _wH) * 2.0f;
}

static float rad2deg(float rad) {
        return rad * 180 / M_PI;
}

static void mousepressed(int button, int state, int mouse_x, int mouse_y) {
        if(_state == MENU) {
                mousepressed_launcher(button, state, mouse_x, mouse_y);
        } else {
                if (button == 1 && state == 1) {
                        float mcoords[2] = {mouse_x, mouse_y};
                        sdl2gl(mcoords);

                        GLfloat m[16], tmpp[16], tmpv[16], *gl4dm;
                        /* copie de la matrice de projection dans tmpp */
                        gl4duBindMatrix("projectionMatrix");
                        gl4dm = gl4duGetMatrixData();
                        memcpy(tmpp, gl4dm, sizeof tmpp);
                        /* copie de la matrice de view dans tmpv */
                        gl4duBindMatrix("viewMatrix");
                        gl4dm = gl4duGetMatrixData();
                        memcpy(tmpv, gl4dm, sizeof tmpv);
                        /* m est tmpp x tmpm */
                        MMAT4XMAT4(m, tmpp, tmpv);
                        MMAT4INVERSE(m);

                        float from[4], to[4];
                        float pos[4] = {0.0f, -0.5f, -1.0f, 1.0f};
                        MMAT4XVEC4(from, m, pos);
                        float pos2[4] = {mcoords[0], mcoords[1], 1.0f, 1.0f};
                        MMAT4XVEC4(to, m, pos2);
                        MVEC4WEIGHT(to);

                        for (int i = 0; i < 3; i++) {
                                from[i] /= from[3];
                                to[i] /= to[3];
                        }

                        shootf(from, to);

                } else if (button == 3 && state == 1) {
                        hypervelocity = 100;
                }
        }
}

/**
 * Dessiner tout
 **/
static void draw() {
        if(_state == MENU) {
                draw_launcher();
        } else if(_state == CREDITS) {
                draw_credit();
        } else {
                float vector_view[3] = {_cam.x, _cam.y, _cam.z - 1};
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                /* Affichage dans l'espace */
                if (on_spaceship == 1) {
                        gl4duBindMatrix("viewMatrix");
                        gl4duLoadIdentityf();
                        /* Vu exterieur */
                        if (view == 0)
                                gl4duLookAtf(_cam.x, _cam.y, _cam.z, _cam.x - sin(_cam.theta),
                                             _cam.y - angleY, _cam.z - cos(_cam.theta), 0.0, 1.0, 0.0);
                        /* Vu interieur */
                        else if (view == 1)
                                gl4duLookAtf(_cam.x, _cam.y + 0.18, _cam.z, _cam.x - sin(_cam.theta),
                                             _cam.y - tan(angleY * 3), _cam.z - cos(_cam.theta), 0.0, 1.0,
                                             0.0);

                        draw_skybox(_cam.x, _cam.y, _cam.z);
                        draw_space(_light);
                        if (hypervelocity > 0) {
                                glUseProgram(_pModelId);
                                gl4duBindMatrix("modelMatrix");
                                gl4duLoadIdentityf();

                                gl4duTranslatef(_cam.x - sin(_cam.theta), _cam.y - 0.18 - angleY,
                                                _cam.z - cos(_cam.theta));
                                gl4duRotatef(rad2deg(_cam.theta) + 180.0, 0, 1, 0);
                                gl4duRotatef(rad2deg(angleY), 1, 0, 0);
                                gl4duRotatef(-10, 1, 0, 0);
                                gl4duScalef(2.0 / 5.0, 2.0 / 5.0, 2.0 / 5.0);

                                glUniform3fv(glGetUniformLocation(_pModelId, "vector_view"), 1,
                                             vector_view);
                                assimpDrawScene();

                                animate_hypervelocity();
                                hypervelocity--;
                        } else if (hypervelocity == 0) {
                                draw_shoot();

                                if (view == 0) {
                                        glUseProgram(_pModelId);
                                        gl4duBindMatrix("modelMatrix");
                                        gl4duLoadIdentityf();

                                        gl4duTranslatef(_cam.x - sin(_cam.theta), _cam.y - 0.18 - angleY,
                                                        _cam.z - cos(_cam.theta));
                                        gl4duRotatef(rad2deg(_cam.theta) + 180.0, 0, 1, 0);
                                        gl4duRotatef(rad2deg(angleY), 1, 0, 0);
                                        gl4duRotatef(-10, 1, 0, 0);
                                        gl4duScalef(2.0 / 5.0, 2.0 / 5.0, 2.0 / 5.0);

                                        gl4duRotatef(-rad2deg(xClip), 0, 1, 0);
                                        gl4duRotatef(-rad2deg(yClip), 1, 0, 0);

                                        apply_stars();
                                        glUniform3fv(glGetUniformLocation(_pModelId, "vector_view"), 1,
                                                     vector_view);
                                        assimpDrawScene();

                                        draw_crosshair(xClip, yClip);
                                } else if (view == 1) {
                                        draw_cockpit();
                                }
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

                if (interact != 0) {
                        draw_interact();
                }

                if (on_spaceship == 1) {
                        /**
                         * Partie Reseau
                         **/

                        /* Transforme les coordonnees du joueur en message a envoye */
                        vector3 coordinates = {_cam.x, _cam.y, _cam.z};
                        char *message = struct2str(1, coordinates);

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
                                glUniform3fv(glGetUniformLocation(_pModelId, "vector_view"), 1,
                                             vector_view);
                                assimpDrawScene();

                                glUseProgram(0);
                        }
                }

                glEnable(GL_DEPTH_TEST);
                glEnable(GL_CULL_FACE);
        }
}

static void quit(void) {
        assimpQuit();
        quit_crosshair();
        quit_space();
        quit_skybox();
        gl4duClean(GL4DU_ALL);
}

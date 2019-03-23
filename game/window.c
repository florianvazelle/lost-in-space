#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>

#include "crosshair.h"
#include "space.h"
#include "skybox.h"
#include "cockpit.h"
#include "interact.h"

#include "utils/load_texture.h"

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
                GLuint _planetTexId = hit_player_satellite(player);
                if(_planetTexId == 0) {
                        _cam.x = player.data.x + player.dir.x;
                        _cam.y = player.data.y + player.dir.y;
                        _cam.z = player.data.z + player.dir.z;
                }
                interact = _planetTexId;
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
        /* Debut */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(on_spaceship == 1) {
                gl4duBindMatrix("viewMatrix");
                gl4duLoadIdentityf();
                if(view == 0)
                        gl4duLookAtf(_cam.x, _cam.y, _cam.z,
                                     _cam.x - sin(_cam.theta), _cam.y - angleY,  _cam.z - cos(_cam.theta),
                                     0.0, 1.0, 0.0);
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

                        glUseProgram(_pBasicId);
                        glUniform1i(glGetUniformLocation(_pBasicId, "phong"), 0);
                        draw_crosshair(xClip, yClip);

                        glUseProgram(0);
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
                gl4duPushMatrix(); {
                        gl4duRotatef(-90, 1, 0, 0);
                        gl4duScalef(100, 100, 1);
                        gl4duSendMatrices();
                } gl4duPopMatrix();
                glBindTexture(GL_TEXTURE_2D, interact);
                gl4dgDraw(_plane);
                glUseProgram(0);
        }

        if(interact != 0) {
                draw_interact(_pBasicId);
        }

        /* enables cull facing and depth testing */
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

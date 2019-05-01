#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>

#include "cockpit.h"
#include "config.h"
#include "crosshair.h"
#include "interact.h"
#include "landed.h"
#include "p2p.h"
#include "skybox.h"
#include "space.h"

#include "hypervelocity.h"
#include "laser.h"

#include "utils/convert.h"
#include "utils/load_texture.h"

static int _wW = 800, _wH = 600;
static int _xm = 400, _ym = 300;

static float xClip, yClip;
static GLfloat angleY;
static int view = 0;

static int hypervelocity = 0;
static int on_spaceship = 1;
static GLuint interact = 0;

enum kyes_t { KLEFT = 0, KRIGHT, KUP, KDOWN };

static GLuint _keys[] = {0, 0, 0, 0};

/*typedef struct  {
        GLfloat x, y, z;
        GLfloat theta;
   } cam_t;*/

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
  if (!gl4duwCreateWindow(argc, argv, "Lost in Space!", 10, 10, _wW, _wH,
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
    exit(0);
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

static void mousepressed(int button, int state, int mouse_x, int mouse_y) {
  if (button == 1 && state == 1) {
    // float xGL = ((x + 0.5f) / _wW) * 2.0f - 1.0f;
    // float yGL = (1.0f - ((y + 0.5f) / _wH) * 2.0f) - 0.5f;

    /*  float x2 = _cam.x + (sin(_cam.theta) * 100);
       float y2 = _cam.y + (tan(angleY) * 100);
       float z2 = _cam.z + (cos(_cam.theta) * 100);

       float deltaX = x2 - _cam.x;
       float deltaY = y2 - _cam.y;
       float deltaZ = z2 - _cam.z;

       float len = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
       deltaX = deltaX / len;
       deltaY = deltaY / len;
       deltaZ = deltaZ / len;

       printf("angleX: %.02lf - angleY: %.02lf - angleZ: %.02lf\n",
       sin(_cam.theta), tan(angleY), cos(_cam.theta) );
       //float x_an = (_cam.theta * 180.0 / M_PI) + 180.0;
       //float y_an = angleY * 180.0 / M_PI;
       //float z_an = (_cam.theta * 180.0 / M_PI);
       shoot(_cam, x2, y2, z2, deltaX, deltaY, deltaZ);*/
    /*GLfloat *matV, *matP, *_mat;
       gl4duBindMatrix("viewMatrix");
       matV = gl4duGetMatrixData();
       gl4duBindMatrix("projectionMatrix");
       matP = gl4duGetMatrixData();
       MMAT4XMAT4(_mat, matP, matV);
       MMAT4INVERSE(_mat);

       GLfloat in[4], pos[4];
       float winZ = 1.0f;

       in[0] = ((x + 0.5f) / _wW) * 2.0f - 1.0f;
       in[1] = (1.0f - ((y + 0.5f) / _wH) * 2.0f);
       in[2] = 2.0f * winZ - 1.0f;
       in[3] = 100.0f;

       MMAT4XVEC4(pos, _mat, in);

       pos[3] = 1.0 / pos[3];

       pos[0] *= pos[3];
       pos[1] *= pos[3];
       pos[2] *= pos[3];

       float x2 = _cam.x + pos[0];
       float y2 = _cam.y - pos[1];
       float z2 = _cam.z + pos[2];

       float deltaX = x2 - _cam.x;
       float deltaY = y2 - _cam.y;
       float deltaZ = z2 - _cam.z;

       float len = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
       deltaX = deltaX / len;
       deltaY = deltaY / len;
       deltaZ = deltaZ / len;

       printf("x1: %.02lf - y1: %.02lf - z1: %.02lf\n", _cam.x, _cam.y, _cam.z);
       printf("x2: %.02lf - y2: %.02lf - z2: %.02lf\n", x2, y2, z2);
       shoot(_cam, x2, y2, z2, deltaX, deltaY, deltaZ);*/

    // vec2f mouse = (vec2f(mousePosition) / vec2f(windowSize)) * 2.0f - 1.0f;
    //  float xGL = (x / _wW) * 2.0f - 1.0f;
    // float yGL = 1.0f - ((y / _wH) * 2.0f);

    // mat44 toWorld = (camera.projection * camera.transform).inverse();
    //  GLfloat *matV, /**matM,*/ *matP, *_mat; //, *trans;
    //  gl4duBindMatrix("modelMatrix");
    // matM = gl4duGetMatrixData();
    /*gl4duBindMatrix("viewMatrix");
       matV = gl4duGetMatrixData();
       //MMAT4XMAT4(trans, matV, matM);
       gl4duBindMatrix("projectionMatrix");
       matP = gl4duGetMatrixData();
       MMAT4XMAT4(_mat, matP, matV);
       MMAT4INVERSE(_mat);

       float from[4], to[4];
       //vec4f from = toWorld * vec4f(mouse, -1.0f, 1.0f);
       float pos[4] = { 0.0f, -0.5f, 0.0f, 1.0f };
       MMAT4XVEC4(from, _mat, pos);
       //vec4f to = toWorld * vec4f(mouse, 1.0f, 1.0f);
       float pos2[4] = { xGL, yGL, 100.0f, 1.0f };
       MMAT4XVEC4(to, _mat, pos2);

       //from /= from.w;
       //to /= to.w;
       for(int i = 0; i < 3; i++) {
            from[i] /= from[3];
            to[i] /= to[3];
       }

       float angle[3];
       float deltaX = to[0] - from[0];
       float deltaY = to[1] - from[1];
       float deltaZ = to[2] - from[2];


       from[0] = to[0] =_cam.x;
       from[1] = to[1] = _cam.y;
       from[2] = to[2] = _cam.z;

       to[0] += (deltaX * 10.f);
       to[1] += (deltaY * 10.f);
       to[2] += (deltaZ * 10.f);

       deltaX = to[0] - from[0];
       deltaY = to[1] - from[1];
       deltaZ = to[2] - from[2];

       float len = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
       angle[0] = deltaX / len;
       angle[1] = deltaY / len;
       angle[2] = deltaZ / len;

       shootf(from, to, angle);*/
    GLfloat m[16], tmpp[16], tmpm[16], *gl4dm;
    GLfloat mcoords[] = {_cam.x, _cam.y, _cam.z, 1};

    float x = (2.0f * mouse_x) / _wW - 1.0f;
    float y = 1.0f - (2.0f * mouse_y) / _wH;
    float z = 1.0f;

    float ray_nds[3] = {x, y, z};
    float ray_clip[4] = {x, y, -z, 100.0f};

    gl4duBindMatrix("projectionMatrix");
    gl4dm = gl4duGetMatrixData();
    memcpy(tmpp, gl4dm, sizeof tmpp);
    MMAT4INVERSE(tmpp);

    float ray_eye[4];
    MMAT4XVEC4(ray_eye, tmpp, ray_clip);
    ray_eye[3] = 0.0f;

    gl4duBindMatrix("viewMatrix");
    gl4dm = gl4duGetMatrixData();
    memcpy(tmpm, gl4dm, sizeof tmpm);
    MMAT4INVERSE(tmpm);

    float ray_wor[4];
    MMAT4XVEC4(ray_wor, tmpm, ray_eye);
    MVEC4WEIGHT(ray_wor);

    // float deltaX = ray_wor[0] - _cam.x;
    // float deltaY = ray_wor[1] - _cam.y;
    // float deltaZ = ray_wor[2] - _cam.z;

    // float len = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
    // deltaX = deltaX / len;
    // deltaY = deltaY / len;
    // deltaZ = deltaZ / len;
    printf("Lazer Coord: %lf %lf %lf\n", ray_wor[0], ray_wor[1], ray_wor[2]);

    // shoot(_cam, ray_wor[0], ray_wor[1], ray_wor[2], deltaX, deltaY,
    // deltaZ);

    /*float xGL = (mouse_x / _wW) * 2.0f - 1.0f;
    float yGL = 1.0f - ((mouse_y / _wH) * 2.0f);

    // mat44 toWorld = (camera.projection * camera.transform).inverse();
    GLfloat _mat[16], m[16], tmpp[16], tmpm[16], tmpv[16], *gl4dm;
    // gl4duBindMatrix("modelMatrix");
    // gl4dm = gl4duGetMatrixData();
    // memcpy(tmpm, gl4dm, sizeof tmpp);
    gl4duBindMatrix("viewMatrix");
    gl4dm = gl4duGetMatrixData();
    memcpy(tmpv, gl4dm, sizeof tmpp);
    // MMAT4XMAT4(m, tmpv, tmpm);
    gl4duBindMatrix("projectionMatrix");
    gl4dm = gl4duGetMatrixData();
    memcpy(tmpp, gl4dm, sizeof tmpp);
    MMAT4XMAT4(_mat, tmpp, tmpv);
    MMAT4INVERSE(_mat);

    float from[4], to[4];
    // vec4f from = toWorld * vec4f(mouse, -1.0f, 1.0f);
    float pos[4] = {0.0f, -0.5f, 0.0f, 1.0f};
    MMAT4XVEC4(from, _mat, pos);
    // vec4f to = toWorld * vec4f(mouse, 1.0f, 1.0f);
    float pos2[4] = {xGL, yGL, 100.0f, 1.0f};
    MMAT4XVEC4(to, _mat, pos2);
    MVEC4WEIGHT(to);
    printf("Lazer Coord: %lf %lf %lf\n", to[0], to[1], to[2]);*/

  } else if (button == 3 && state == 1) {
    hypervelocity = 100;
  }
}

/**
 * Dessiner tout
 **/
static void draw() {
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
      gl4duRotatef((_cam.theta * 180.0 / M_PI) + 180.0, 0, 1, 0);
      gl4duRotatef((angleY * 180.0 / M_PI), 1, 0, 0);
      gl4duRotatef(-10, 1, 0, 0);
      gl4duScalef(2.0 / 5.0, 2.0 / 5.0, 2.0 / 5.0);

      glUniform3fv(glGetUniformLocation(_pModelId, "vector_view"), 1,
                   vector_view);
      assimpDrawScene();

      animate_hypervelocity(hypervelocity);
      hypervelocity--;
    } else if (hypervelocity == 0) {
      draw_shoot();

      if (view == 0) {
        glUseProgram(_pModelId);
        gl4duBindMatrix("modelMatrix");
        gl4duLoadIdentityf();

        gl4duTranslatef(_cam.x - sin(_cam.theta), _cam.y - 0.18 - angleY,
                        _cam.z - cos(_cam.theta));
        gl4duRotatef((_cam.theta * 180.0 / M_PI) + 180.0, 0, 1, 0);
        gl4duRotatef((angleY * 180.0 / M_PI), 1, 0, 0);
        gl4duRotatef(-10, 1, 0, 0);
        gl4duScalef(2.0 / 5.0, 2.0 / 5.0, 2.0 / 5.0);

        gl4duRotatef(-(xClip * 180.0 / M_PI), 0, 1, 0);
        gl4duRotatef(-(yClip * 180.0 / M_PI), 1, 0, 0);

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

static void quit(void) {
  assimpQuit();
  quit_crosshair();
  quit_space();
  quit_skybox();
  gl4duClean(GL4DU_ALL);
}

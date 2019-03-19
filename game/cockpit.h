#ifndef _COCKPIT_H
#define _COCKPIT_H

#include <GL4D/gl4dp.h>

static GLuint _planeC = 0;
static GLuint _cockpitTexId = 0;

void init_cockpit() {
  _planeC = gl4dgGenQuadf();

  SDL_Surface *t;
  glGenTextures(1, &_cockpitTexId);
  glBindTexture(GL_TEXTURE_2D, _cockpitTexId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if ((t = IMG_Load("assets/textures/cockpit.png")) != NULL) {
#ifdef __APPLE__
    int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode,
                 GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file assets/textures/cockpit.png : %s\n",
            SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 NULL);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_cockpit(double posx, double posy) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  gl4duBindMatrix("projectionMatrix");
  gl4duPushMatrix();
  {
    gl4duLoadIdentityf();
    gl4duBindMatrix("modelMatrix");
    gl4duPushMatrix();
    {
      gl4duLoadIdentityf();
      gl4duRotatef(180, 0, 0, 1);
      gl4duBindMatrix("viewMatrix");
      gl4duPushMatrix();
      {
        gl4duLoadIdentityf();
        gl4duSendMatrices();
      }
      gl4duPopMatrix();
      gl4duBindMatrix("modelMatrix");
    }
    gl4duPopMatrix();
    gl4duBindMatrix("projectionMatrix");
  }
  gl4duPopMatrix();
  gl4duBindMatrix("modelMatrix");

  glBindTexture(GL_TEXTURE_2D, _cockpitTexId);
  gl4dgDraw(_planeC);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void quit_cockpit() {
  if (_cockpitTexId)
    glDeleteTextures(1, &_cockpitTexId);
}

#endif

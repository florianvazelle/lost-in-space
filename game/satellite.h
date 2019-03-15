#ifndef _SATELLITE_H
#define _SATELLITE_H

#include <GL4D/gl4dp.h>
#include <SDL_image.h>

typedef struct {
  float x, y, z;
  float width;
  const char *type;
  const char *sprite;
} satellite;

static GLuint _sphere;
static GLuint _planetTexId = 0;
static GLuint _asteroidTexId = 0;

static void bindTexture(satellite s) {
  if (strcmp(s.type, "planet") == 0) {
    glBindTexture(GL_TEXTURE_2D, _planetTexId);
  }
  if (strcmp(s.type, "asteroid") == 0) {
    glBindTexture(GL_TEXTURE_2D, _asteroidTexId);
  }
}

void init_satellite(satellite s) {
  _sphere = gl4dgGenSpheref(50, 50);

  SDL_Surface *t;
  bindTexture(s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if ((t = IMG_Load(s.sprite)) != NULL) {
#ifdef __APPLE__
    int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode,
                 GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file %s : %s\n", s.sprite, SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 NULL);
  }
}

void draw_satellite(satellite s) {
  gl4duBindMatrix("modelMatrix");
  gl4duLoadIdentityf();
  glEnable(GL_TEXTURE_2D);
  bindTexture(s);
  gl4duPushMatrix();
  {
    gl4duTranslatef(s.x, s.y, s.z);
    gl4duScalef(s.width, s.width, s.width);
    gl4duSendMatrices();
  }
  gl4duPopMatrix();
  gl4dgDraw(_sphere);
  glDisable(GL_TEXTURE_2D);
}

void quit_satellite() {
  if (_planetTexId)
    glDeleteTextures(1, &_planetTexId);
  if (_asteroidTexId)
    glDeleteTextures(1, &_asteroidTexId);
}

#endif

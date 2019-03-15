#ifndef _ASTEROID_H
#define _ASTEROID_H

#include <GL4D/gl4dp.h>
#include <SDL_image.h>

typedef struct {
  float x, y, z;
  float width;
  const char *sprite;
} asteroid;

static GLuint _sphereA = 0;
static GLuint _asteroidTexId = 0;

void init_asteroid(asteroid a) {
  _sphereA = gl4dgGenSpheref(50, 50);

  SDL_Surface *t;
  glBindTexture(GL_TEXTURE_2D, _asteroidTexId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if ((t = IMG_Load(a.sprite)) != NULL) {
#ifdef __APPLE__
    int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode,
                 GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file %s : %s\n", a.sprite, SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 NULL);
  }
}

void draw_asteroid(asteroid a) {
  gl4duBindMatrix("modelMatrix");
  gl4duLoadIdentityf();
  glBindTexture(GL_TEXTURE_2D, _asteroidTexId);
  gl4duPushMatrix();
  {
    gl4duTranslatef(a.x, a.y, a.z);
    gl4duScalef(a.width, a.width, a.width);
    gl4duSendMatrices();
  }
  gl4duPopMatrix();
  gl4dgDraw(_sphereA);
}

void quit_asteroid() {
  if (_asteroidTexId)
    glDeleteTextures(1, &_asteroidTexId);
}

#endif

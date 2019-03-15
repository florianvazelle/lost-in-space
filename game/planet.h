#ifndef _PLANET_H
#define _PLANET_H

#include <GL4D/gl4dp.h>
#include <SDL_image.h>

typedef struct {
  float x, y, z;
  float width;
  const char *sprite;
} planet;

static GLuint _sphereP;
static GLuint _planetTexId = 0;

void init_planet(planet p) {
  _sphereP = gl4dgGenSpheref(50, 50);

  SDL_Surface *t;
  glBindTexture(GL_TEXTURE_2D, _planetTexId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  if ((t = IMG_Load(p.sprite)) != NULL) {
#ifdef __APPLE__
    int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
    int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode,
                 GL_UNSIGNED_BYTE, t->pixels);
    SDL_FreeSurface(t);
  } else {
    fprintf(stderr, "can't open file %s : %s\n", p.sprite, SDL_GetError());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 NULL);
  }
}

void draw_planet(planet p) {
  gl4duBindMatrix("modelMatrix");
  gl4duLoadIdentityf();
  glBindTexture(GL_TEXTURE_2D, _planetTexId);
  gl4duPushMatrix();
  {
    gl4duTranslatef(p.x, p.y, p.z);
    gl4duScalef(p.width, p.width, p.width);
    gl4duSendMatrices();
  }
  gl4duPopMatrix();
  gl4dgDraw(_sphereP);
}

void quit_planet() {
  if (_planetTexId)
    glDeleteTextures(1, &_planetTexId);
}

#endif

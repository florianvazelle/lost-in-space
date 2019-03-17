#ifndef _SATELLITE_H
#define _SATELLITE_H

#include <GL4D/gl4dp.h>
#include <SDL_image.h>

typedef struct {
  float x, y, z;
  float width;
  const char *type;
  const char *sprite;
  GLuint _texId;
} satellite;

static GLuint _sphere = 0;

GLuint init_satellite(satellite s) {
  if (_sphere == 0)
    _sphere = gl4dgGenSpheref(50, 50);

  if (s._texId == 0) {
    SDL_Surface *t;
    glGenTextures(1, &s._texId);
    glBindTexture(GL_TEXTURE_2D, s._texId);
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
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, NULL);
    }
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  return s._texId;
}

void draw_satellite(satellite s, GLuint _pBasicId) {
  glUseProgram(_pBasicId);
  gl4duBindMatrix("modelMatrix");
  gl4duLoadIdentityf();
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(glGetUniformLocation(_pBasicId, "tex"), 0);
  gl4duPushMatrix();
  {
    gl4duTranslatef(s.x, s.y, s.z);
    gl4duScalef(s.width, s.width, s.width);
    gl4duSendMatrices();
  }
  gl4duPopMatrix();
  // glCullFace(GL_BACK);
  glBindTexture(GL_TEXTURE_2D, s._texId);
  glUniform1f(glGetUniformLocation(_pBasicId, "texRepeat"), 1.0);
  gl4dgDraw(_sphere);
}

void quit_satellite(satellite s) {
  if (s._texId)
    glDeleteTextures(1, &s._texId);
}

#endif

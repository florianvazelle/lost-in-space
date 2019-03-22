#ifndef _SATELLITE_H
#define _SATELLITE_H

#include <GL4D/gl4dp.h>

typedef struct {
  int id;
  float x, y, z;
  float width;
  const char *path_sprite;
  GLuint _texId;
} satellite;

GLuint init_satellite(satellite);
void draw_satellite(satellite, GLuint, GLuint, GLfloat[3]);
void quit_satellite(satellite);

#endif

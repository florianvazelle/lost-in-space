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

GLuint init_satellite(satellite s);
void draw_satellite(satellite s, GLuint _pBasicId);
void quit_satellite(satellite s);

#endif

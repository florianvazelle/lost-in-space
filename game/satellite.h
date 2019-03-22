#ifndef _SATELLITE_H
#define _SATELLITE_H

#include "utils/sphere.h"
#include <GL4D/gl4dp.h>

typedef struct {
  int id;
  sphere data;
  vector3 dir;
  const char *path_sprite;
  GLuint _texId;
} satellite;

GLuint init_satellite(satellite);
vector3 update_satellite(satellite);
void draw_satellite(satellite, GLuint, GLuint);
void quit_satellite(satellite);
int hit_satellite(satellite, satellite);

#endif

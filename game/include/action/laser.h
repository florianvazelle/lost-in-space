#ifndef _LASER_H
#define _LASER_H

#include "config.h"

typedef struct {
  GLfloat x, y, z;
  GLfloat theta;
} cam_t;

void shootf(float[4], float[4]);
void draw_shoot();

#endif

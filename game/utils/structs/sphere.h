#ifndef _SPHERE_H
#define _SPHERE_H

#include "vector3.h"

typedef struct _sphere sphere;
struct _sphere {
  float x, y, z;
  float rayon;
};

int hit_sphere_sphere(sphere, sphere);
sphere apply_dir(sphere, vector3);

#endif

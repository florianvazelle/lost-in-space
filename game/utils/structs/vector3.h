#ifndef _VECTOR_H
#define _VECTOR_H

typedef struct _vector3 vector3;
struct _vector3 {
  float x, y, z;
};

vector3 zero();
vector3 invert(vector3);
vector3 inertia();

float norme(vector3);
float distance(vector3, vector3);
float pscal(vector3, vector3);
float angle(vector3, vector3);

#endif

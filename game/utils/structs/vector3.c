#include <math.h>

#include "vector3.h"

vector3 zero() {
  vector3 r = {0, 0, 0};
  return r;
}

vector3 invert(vector3 v) {
  vector3 r = {-v.x, -v.y, -v.z};
  return r;
}

vector3 inertia() {
  vector3 r = {0.02f, 0.01f, 0.03f};
  return r;
}

float norme(vector3 a) {
  return sqrt((a.x * a.x) * +(a.y * a.y) + (a.z * a.z));
}

float distance(vector3 a, vector3 b) {
  return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) +
              (a.z - b.z) * (a.z - b.z));
}

float pscal(vector3 A, vector3 B) { return A.x * B.x + A.y * B.y + A.z * B.z; }

float angle(vector3 a, vector3 b) {
  return acos(pscal(a, b) / (norme(a) * norme(b)));
}

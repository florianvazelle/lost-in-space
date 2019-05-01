#include "util/struct/sphere.h"

int hit_sphere_sphere(sphere S1, sphere S2) {
  int d2 = (S1.x - S2.x) * (S1.x - S2.x) + (S1.y - S2.y) * (S1.y - S2.y) +
           (S1.z - S2.z) * (S1.z - S2.z);
  if (d2 > (S1.rayon + S2.rayon) * (S1.rayon + S2.rayon)) {
    return 0;
  } else {
    return 1;
  }
}

static int CollisionPointCercle(vector3 p, sphere S) {
  int d2 = (p.x - S.x) * (p.x - S.x) + (p.y - S.y) * (p.y - S.y) +
           (p.z - S.z) * (p.z - S.z);
  if (d2 > S.rayon * S.rayon)
    return 0;
  else
    return 1;
}

int hit_line_sphere(vector3 A, vector3 B, sphere C) {
  vector3 AB, AC;
  AB.x = B.x - A.x;
  AB.y = B.y - A.y;
  AB.z = B.z - A.z;
  AC.x = C.x - A.x;
  AC.y = C.y - A.y;
  AC.z = C.z - A.z;
  float pscal1 = AB.x * AC.x + AB.y * AC.y + AB.z * AC.z;
  if (pscal1 >= 0)
    return 1;
  if (CollisionPointCercle(A, C))
    return 1;
  return 0;
}

sphere apply_dir(sphere s, vector3 v) {
  sphere r = {s.x + v.x, s.y + v.y, s.z + v.z, s.rayon};
  return r;
}

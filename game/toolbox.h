#include <math.h>

double getDistance3D(float x1, float y1, float z1, float x2, float y2,
                     float z2) {
  double x = (x2 - x1) * (x2 - x1);
  double y = (y2 - y1) * (y2 - y1);
  double z = (z2 - z1) * (z2 - z1);

  return sqrt(x + y + z);
}

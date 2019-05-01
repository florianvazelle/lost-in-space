#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "util/struct/planet.h"

static double getDistance3D(float x1, float y1, float z1, float x2, float y2,
                            float z2) {
  double x = (x2 - x1) * (x2 - x1);
  double y = (y2 - y1) * (y2 - y1);
  double z = (z2 - z1) * (z2 - z1);
  return sqrt(x + y + z);
}

static void fusion(satellite tableau[], int deb1, int fin1, int fin2, int x,
                   int y, int z) {
  satellite *table1;
  int deb2 = fin1 + 1;
  int compt1 = deb1;
  int compt2 = deb2;
  int i;

  table1 = malloc((fin1 - deb1 + 1) * sizeof(satellite));

  for (i = deb1; i <= fin1; i++) {
    table1[i - deb1] = tableau[i];
  }

  for (i = deb1; i <= fin2; i++) {
    if (compt1 == deb2) {
      break;
    } else if (compt2 == (fin2 + 1)) {
      tableau[i] = table1[compt1 - deb1];
      compt1++;
    } else {
      double v1 = getDistance3D(table1[compt1 - deb1].body.data.x,
                                table1[compt1 - deb1].body.data.y,
                                table1[compt1 - deb1].body.data.z, x, y, z);
      double v2 = getDistance3D(tableau[compt2].body.data.x,
                                tableau[compt2].body.data.y,
                                tableau[compt2].body.data.z, x, y, z);
      if (v1 < v2) {
        tableau[i] = table1[compt1 - deb1];
        compt1++;
      } else {
        tableau[i] = tableau[compt2];
        compt2++;
      }
    }
  }
  free(table1);
}

static void tri_fusion_bis(satellite tableau[], int deb, int fin, int x, int y,
                           int z) {
  if (deb != fin) {
    int milieu = (fin + deb) / 2;
    tri_fusion_bis(tableau, deb, milieu, x, y, z);
    tri_fusion_bis(tableau, milieu + 1, fin, x, y, z);
    fusion(tableau, deb, milieu, fin, x, y, z);
  }
}

void tri_fusion(satellite tableau[], int length, int x, int y, int z) {
  if (length > 0)
    tri_fusion_bis(tableau, 0, length - 1, x, y, z);
}

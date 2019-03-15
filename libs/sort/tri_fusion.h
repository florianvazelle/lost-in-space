#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../game/satellite.h"

double getDistance3D(float x1, float y1, float z1, float x2, float y2,
                     float z2) {
  double x = (x2 - x1) * (x2 - x1);
  double y = (y2 - y1) * (y2 - y1);
  double z = (z2 - z1) * (z2 - z1);

  return sqrt(x + y + z);
}

void interchange(satellite tab1[], satellite tab2[], int idx1, int idx2) {
  tab1[idx1].x = tab2[idx2].x;
  tab1[idx1].y = tab2[idx2].y;
  tab1[idx1].z = tab2[idx2].z;
  tab1[idx1].width = tab2[idx2].width;
  tab1[idx1].type = tab2[idx2].type;
  tab1[idx1].sprite = tab2[idx2].sprite;
}

void fusion(satellite tableau[], int deb1, int fin1, int fin2, int x, int y,
            int z) {
  satellite *table1;
  int deb2 = fin1 + 1;
  int compt1 = deb1;
  int compt2 = deb2;
  int i;

  table1 = malloc((fin1 - deb1 + 1) * sizeof(satellite));

  for (i = deb1; i <= fin1; i++) {
    interchange(table1, tableau, i - deb1, i);
  }

  for (i = deb1; i <= fin2; i++) {
    if (compt1 == deb2) {
      break;
    } else if (compt2 == (fin2 + 1)) {
      interchange(tableau, table1, i, compt1 - deb1);
      compt1++;
    } else {
      double v1 =
          getDistance3D(table1[compt1 - deb1].x, table1[compt1 - deb1].y,
                        table1[compt1 - deb1].z, x, y, z);
      double v2 = getDistance3D(tableau[compt2].x, tableau[compt2].y,
                                tableau[compt2].z, x, y, z);
      if (v1 < v2) {
        interchange(tableau, table1, i, compt1 - deb1);
        compt1++;
      } else {
        interchange(tableau, tableau, i, compt2);
        compt2++;
      }
    }
  }
  free(table1);
}

void tri_fusion_bis(satellite tableau[], int deb, int fin, int x, int y,
                    int z) {
  if (deb != fin) {
    int milieu = (fin + deb) / 2;
    tri_fusion_bis(tableau, deb, milieu, x, y, z);
    tri_fusion_bis(tableau, milieu + 1, fin, x, y, z);
    fusion(tableau, deb, milieu, fin, x, y, z);
  }
}

void tri_fusion(satellite tableau[], int length, int x, int y, int z) {
  if (length > 0) {
    tri_fusion_bis(tableau, 0, length - 1, x, y, z);
  }
}

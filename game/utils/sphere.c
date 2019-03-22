#include "sphere.h"

int hit_sphere_sphere(sphere S1, sphere S2) {
        int d2 = (S1.x - S2.x) * (S1.x - S2.x) + (S1.y - S2.y) * (S1.y - S2.y) + (S1.z - S2.z) * (S1.z - S2.z);
        if(d2 > (S1.rayon + S2.rayon) * (S1.rayon + S2.rayon)) {
                return 0;
        } else {
                return 1;
        }
}

sphere apply_dir(sphere s, vector3 v){
        sphere r = { s.x + v.x, s.y + v.y, s.z + v.z, s.rayon };
        return r;
}

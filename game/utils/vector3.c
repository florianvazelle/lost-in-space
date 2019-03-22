#include "vector3.h"

vector3 zero() {
        vector3 r = {0, 0, 0};
        return r;
}

vector3 invert(vector3 v) {
        vector3 r = {-v.x, -v.y, -v.z};
        return r;
}

#ifndef _ENTITIE_H
#define _ENTITIE_H

#include "sphere.h"

typedef struct _entitie entitie;
struct _entitie {
  sphere data;
  vector3 dir;
};

#endif

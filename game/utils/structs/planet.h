#ifndef _PLANET_H
#define _PLANET_H

#include <GL4D/gl4dp.h>

#include "entitie.h"

typedef struct _satellite satellite;
struct _satellite {
  int id;
  entitie body;
  const char *path_sprite;
  GLuint _texId;
  int collision;
  float explosion;
};

#endif

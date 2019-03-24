#ifndef _CONFIG_H
#define _CONFIG_H

#include <GL4D/gl4dp.h>

/* ID des programmes shaders */
GLuint _pBasicId, _pModelId;

/* ID des figures geometriques */
GLuint _plane, _sphere;

/* Autres */
int _light;

void init_config();

#endif

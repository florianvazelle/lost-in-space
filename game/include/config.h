#ifndef _CONFIG_H
#define _CONFIG_H

#include <GL4D/gl4dp.h>

/* ID des programmes shaders */
GLuint _pBasicId, _pModelId, _pStarfieldId;

/* ID des figures geometriques */
GLuint _plane, _sphere, _cylinder;

/* Shaders */
int _light;

/* Autres */
enum type_keyboard { AZERTY, QWERTY } _keyboard;

void init_config();

#endif

#ifndef _CONFIG_H
#define _CONFIG_H

#include <GL4D/gl4dp.h>

/* ID des programmes shaders */
GLuint _pBasicId, _pModelId, _pStarfieldId, _pTextId;

/* ID des figures geometriques */
GLuint _plane, _sphere, _cylinder;

/* Taille de l'ecran */
int _wW, _wH;

/* Position de la souris */
int _xm, _ym;

/* Shaders */
int _light;

/* Autres */
enum type_state { MENU, PLAY, SETTINGS, CREDITS } _state;

void init_config();

#endif

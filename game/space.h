#ifndef _SPACE_H
#define _SPACE_H

#include "utils/structs/entitie.h"

void init_space(void);
void update_space(int, int, int);
void draw_space(GLuint, GLuint);
void apply_stars(int);
GLuint hit_player_satellite(entitie);
void quit_space(void);

#endif

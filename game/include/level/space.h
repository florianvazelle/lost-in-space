#ifndef _SPACE_H
#define _SPACE_H

#include "util/struct/entitie.h"

void init_space(void);
void update_space(int, int, int);
void draw_space();
void apply_stars(GLuint);

int hit_player_satellite(entitie);
GLuint hit_interact(sphere);
void hit_shoot_satellite(vector3, vector3);

void quit_space(void);

void explose_test(void);

#endif

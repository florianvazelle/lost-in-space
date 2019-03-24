#ifndef _SATELLITE_H
#define _SATELLITE_H

#include "utils/structs/planet.h"

GLuint init_satellite(satellite);
vector3 update_satellite(satellite);
void draw_satellite(satellite);
void quit_satellite(satellite);
int hit_satellite(satellite, satellite);

#endif

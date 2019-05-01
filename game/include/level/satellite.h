#ifndef _SATELLITE_H
#define _SATELLITE_H

#include "util/struct/planet.h"

GLuint init_satellite(satellite);
int update_satellite(satellite *);
void draw_satellite(satellite);
void quit_satellite(satellite);
int hit_satellite(satellite, satellite);
void explose(satellite *);

#endif

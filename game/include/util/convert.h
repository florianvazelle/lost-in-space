#ifndef _CONVERT_H
#define _CONVERT_H

char *struct2str(int, float[3]);
vector3 str2stuct(char *, float *);

char *shoot2str(int, float[3], float[3]);

void sdl2gl(float[2]);
float rad2deg(float);

#endif

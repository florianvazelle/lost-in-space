#ifndef _LOAD_H
#define _LOAD_H

GLuint load_2d_texture(GLuint _texId, GLuint *texture);
GLuint load_2d_sprite_texture(GLuint _texId, const char *path_sprite);
GLuint load_cube_map_sprite_texture(GLuint _texId, const char *path_sprite[6]);

#endif

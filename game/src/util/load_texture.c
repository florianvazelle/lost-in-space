#include <GL4D/gl4dp.h>
#include <SDL_image.h>

void load_2d_texture(GLuint *_texId, GLuint *texture) {
  if (*_texId == 0) {
    glGenTextures(1, _texId);
    glBindTexture(GL_TEXTURE_2D, *_texId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 texture);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void load_2d_sprite_texture(GLuint *_texId, const char *path_sprite) {
  if (*_texId == 0) {
    SDL_Surface *t;
    glGenTextures(1, _texId);
    glBindTexture(GL_TEXTURE_2D, *_texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if ((t = IMG_Load(path_sprite)) != NULL) {
#ifdef __APPLE__
      int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
#else
      int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
#endif
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode,
                   GL_UNSIGNED_BYTE, t->pixels);
      SDL_FreeSurface(t);
    } else {
      fprintf(stderr, "can't open file %s : %s\n", path_sprite, SDL_GetError());
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, NULL);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

void load_cube_map_sprite_texture(GLuint *_texId, const char *path_sprite[6]) {
  if (*_texId == 0) {
    SDL_Surface *xpos = IMG_Load(path_sprite[0]);
    SDL_Surface *xneg = IMG_Load(path_sprite[1]);
    SDL_Surface *ypos = IMG_Load(path_sprite[2]);
    SDL_Surface *yneg = IMG_Load(path_sprite[3]);
    SDL_Surface *zpos = IMG_Load(path_sprite[4]);
    SDL_Surface *zneg = IMG_Load(path_sprite[5]);

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, _texId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *_texId);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos->w, xpos->h,
                 0, xpos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, xpos->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg->w, xneg->h,
                 0, xneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, xneg->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos->w, ypos->h,
                 0, ypos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, ypos->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg->w, yneg->h,
                 0, yneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, yneg->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos->w, zpos->h,
                 0, zpos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, zpos->pixels);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg->w, zneg->h,
                 0, zneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, zneg->pixels);

    SDL_FreeSurface(xneg);
    SDL_FreeSurface(xpos);
    SDL_FreeSurface(yneg);
    SDL_FreeSurface(ypos);
    SDL_FreeSurface(zneg);
    SDL_FreeSurface(zpos);

    glBindTexture(GL_TEXTURE_2D, 0);
  }
}

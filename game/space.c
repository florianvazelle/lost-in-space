#include "satellite.h"
#include "../libs/sort/tri_fusion.h"

#define LENGTH 3

satellite s[] = {
        {0, 0, 0, 100, "planet", "../assets/textures/terre.png"},
        {200, 0, 0, 10, "asteroid", "../assets/textures/asteroid.jpg"},
        {0, 200, 0, 10, "asteroid", "../assets/textures/asteroid.jpg"}
};

static GLuint _spaceTexId = 0;

void draw_galaxy_skybox(int length);

void init_space(){
        for(int i = 0; i < LENGTH; i++) {
                init_satellite(s[i]);
        }

        //_plane = gl4dgGenQuadf();

        SDL_Surface *t;
        glBindTexture(GL_TEXTURE_2D, _spaceTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        if ((t = IMG_Load("../assets/textures/galaxy0.bmp")) != NULL) {
      #ifdef __APPLE__
                int mode = t->format->BytesPerPixel == 4 ? GL_BGRA : GL_BGR;
      #else
                int mode = t->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
      #endif
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, mode,
                             GL_UNSIGNED_BYTE, t->pixels);
                SDL_FreeSurface(t);
        } else {
                fprintf(stderr, "can't open file assets/textures/galaxy.bmp : %s\n", SDL_GetError());
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                             NULL);
        }
}

void update_space(int x, int y, int z){
        tri_fusion(s, LENGTH, x, y, z);
}

void draw_space(){
        draw_galaxy_skybox(100000);
        for (int i = LENGTH - 1; i >= 0; i--) {
                draw_satellite(s[i]);
        }
}

void quit_space(){
        quit_satellite();
}

void draw_galaxy_skybox(int length) {
        glDisable(GL_LIGHTING);
        glDisable(GL_NORMALIZE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _spaceTexId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glColor3fv(get_color(white));
        glBegin(GL_QUADS);
        {
                // Negative X
                {
                        glTexCoord2f(0, 0);
                        glVertex3f(-length, -length, length);
                        glTexCoord2f(1, 0);
                        glVertex3f(-length, -length, -length);
                        glTexCoord2f(1, 1);
                        glVertex3f(-length, length, -length);
                        glTexCoord2f(0, 1);
                        glVertex3f(-length, length, length);
                }

                //  Positive X
                {
                        glTexCoord2f(0, 0);
                        glVertex3f(length, -length, -length);
                        glTexCoord2f(1, 0);
                        glVertex3f(length, -length, length);
                        glTexCoord2f(1, 1);
                        glVertex3f(length, length, length);
                        glTexCoord2f(0, 1);
                        glVertex3f(length, length, -length);
                }

                // Negative Z
                {
                        glTexCoord2f(0, 0);
                        glVertex3f(-length, -length, -length);
                        glTexCoord2f(1, 0);
                        glVertex3f(length, -length, -length);
                        glTexCoord2f(1, 1);
                        glVertex3f(length, length, -length);
                        glTexCoord2f(0, 1);
                        glVertex3f(-length, length, -length);
                }

                // Positive Z
                {
                        glTexCoord2f(0, 0);
                        glVertex3f(length, -length, length);
                        glTexCoord2f(1, 0);
                        glVertex3f(-length, -length, length);
                        glTexCoord2f(1, 1);
                        glVertex3f(-length, length, length);
                        glTexCoord2f(0, 1);
                        glVertex3f(length, length, length);
                }

                // Positive Y
                {
                        glTexCoord2f(0, 0);
                        glVertex3f(-length, length, length);
                        glTexCoord2f(1, 0);
                        glVertex3f(-length, length, -length);
                        glTexCoord2f(1, 1);
                        glVertex3f(length, length, -length);
                        glTexCoord2f(0, 1);
                        glVertex3f(length, length, length);
                }

                // Negative Y
                {
                        glTexCoord2f(0, 0);
                        glVertex3f(-length, -length, -length);
                        glTexCoord2f(1, 0);
                        glVertex3f(-length, -length, length);
                        glTexCoord2f(1, 1);
                        glVertex3f(length, -length, length);
                        glTexCoord2f(0, 1);
                        glVertex3f(length, -length, -length);
                }
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
}

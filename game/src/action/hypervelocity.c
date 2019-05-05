#include <time.h>

#include "config.h"
#include "util/load_texture.h"

static GLuint _texNoise = 0;

static void draw_starfield() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(_pStarfieldId);

        static GLfloat t0 = -1;
        GLfloat t;
        if (t0 < 0.0f)
                t0 = SDL_GetTicks();
        t = (SDL_GetTicks() - t0) / 1000.0f;

        glUniform1i(glGetUniformLocation(_pStarfieldId, "myTexture"), 0);
        glUniform1f(glGetUniformLocation(_pStarfieldId, "getTime"), t);

        gl4duBindMatrix("projectionMatrix");
        gl4duPushMatrix(); {
                gl4duLoadIdentityf();
                gl4duBindMatrix("modelMatrix");
                gl4duPushMatrix(); {
                        gl4duLoadIdentityf();
                        gl4duRotatef(180, 0, 0, 1);
                        gl4duSendMatrices();
                } gl4duPopMatrix();
                gl4duBindMatrix("projectionMatrix");
        } gl4duPopMatrix();
        gl4duBindMatrix("modelMatrix");

        glBindTexture(GL_TEXTURE_2D, _texNoise);
        gl4dgDraw(_plane);

        glDisable(GL_TEXTURE_2D);
        glBlendFunc(GL_ONE, GL_ZERO);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);

}

void animate_hypervelocity() {
        if (_texNoise == 0) {
                load_2d_sprite_texture(&_texNoise, "assets/textures/white-noise.jpg");
        }
        draw_starfield();
}

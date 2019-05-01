#include <time.h>

#include "config.h"
#include "utils/load_texture.h"

static GLuint id = 0;

static void draw_starfield(int hp) {
        glUseProgram(_pStarfieldId);
        glUniform1i(glGetUniformLocation(_pStarfieldId, "myTexture"), 0);
        float f;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform1f(glGetUniformLocation(_pStarfieldId, "getTime"),
                    f = (float)clock() / 100000.0F);
        //printf("%lf\n", f);

        gl4duBindMatrix("projectionMatrix");
        gl4duPushMatrix(); {
                gl4duLoadIdentityf();
                gl4duBindMatrix("modelMatrix");
                gl4duPushMatrix(); {
                        gl4duLoadIdentityf();
                        gl4duRotatef(180, 0, 0, 1);
                        gl4duBindMatrix("viewMatrix");
                        gl4duPushMatrix(); {
                                gl4duLoadIdentityf();
                                gl4duSendMatrices();
                        } gl4duPopMatrix();
                        gl4duBindMatrix("modelMatrix");
                } gl4duPopMatrix();
                gl4duBindMatrix("projectionMatrix");
        } gl4duPopMatrix();
        gl4duBindMatrix("modelMatrix");

        glBindTexture(GL_TEXTURE_2D, id);
        gl4dgDraw(_plane);

        glDisable(GL_TEXTURE_2D);
        glBlendFunc(GL_ONE, GL_ZERO);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
}

void animate_hypervelocity(int hp) {
        if (id == 0) {
                id = load_2d_sprite_texture(id, "assets/textures/white-noise.jpg");
        }
        draw_starfield(hp);
}

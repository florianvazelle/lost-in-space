#include <GL4D/gl4dp.h>

#include "utils/load_texture.h"

static GLuint _planeC = 0;
static GLuint _cockpitTexId = 0;

void init_cockpit() {
        _planeC = gl4dgGenQuadf();
        _cockpitTexId =
                load_2d_sprite_texture(_cockpitTexId, "assets/textures/cockpit.png");
}

void draw_cockpit(double posx, double posy) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        gl4duBindMatrix("projectionMatrix");
        gl4duPushMatrix();
        {
                gl4duLoadIdentityf();
                gl4duBindMatrix("modelMatrix");
                gl4duPushMatrix();
                {
                        gl4duLoadIdentityf();
                        gl4duRotatef(180, 0, 0, 1);
                        gl4duBindMatrix("viewMatrix");
                        gl4duPushMatrix();
                        {
                                gl4duLoadIdentityf();
                                gl4duSendMatrices();
                        }
                        gl4duPopMatrix();
                        gl4duBindMatrix("modelMatrix");
                }
                gl4duPopMatrix();
                gl4duBindMatrix("projectionMatrix");
        }
        gl4duPopMatrix();
        gl4duBindMatrix("modelMatrix");

        glBindTexture(GL_TEXTURE_2D, _cockpitTexId);
        gl4dgDraw(_planeC);

        glDisable(GL_TEXTURE_2D);
        glBlendFunc (GL_ONE, GL_ZERO);
        glDisable (GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
}

void quit_cockpit() {
        if (_cockpitTexId)
                glDeleteTextures(1, &_cockpitTexId);
}

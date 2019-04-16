#include <GL4D/gl4dp.h>

#include "config.h"
#include "utils/load_texture.h"

static GLuint _crosshairTexId = 0;

void init_crosshair() {
        GLuint _crosshairTex[] = {RGB(255, 255, 255)};
        _crosshairTexId = load_2d_texture(_crosshairTexId, _crosshairTex);
}

static void part_crosshair(double rot, double posx, double posy) {
        gl4duBindMatrix("projectionMatrix");
        gl4duPushMatrix(); {
                gl4duLoadIdentityf();
                gl4duBindMatrix("modelMatrix");
                gl4duPushMatrix(); {
                        gl4duLoadIdentityf();
                        gl4duTranslatef(posx, posy, 0.0);
                        gl4duRotatef(rot, 0, 0, 1);
                        gl4duScalef(0.01 / 5.0, 0.2 / 5.0, 0.2 / 5.0);
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

        gl4dgDraw(_plane);
}

void draw_crosshair(float x, float y) {
        glUseProgram(_pBasicId);
        glUniform1i(glGetUniformLocation(_pBasicId, "light"), 0);
        glUniform1f(glGetUniformLocation(_pBasicId, "explosion"), 0);

        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, _crosshairTexId);

        part_crosshair(60.0, -0.1 + x, 0.05 + y);
        part_crosshair(120.0, -0.1 + x, -0.05 + y);
        part_crosshair(-60.0, 0.1 + x, 0.05 + y);
        part_crosshair(-120.0, 0.1 + x, -0.05 + y);

        glUseProgram(0);
}

void quit_crosshair() {
        if (_crosshairTexId)
                glDeleteTextures(1, &_crosshairTexId);
}

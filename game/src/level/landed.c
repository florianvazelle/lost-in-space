#include <GL4D/gl4dp.h>

#include "config.h"

void draw_landed(GLuint interact) {
        glUseProgram(_pBasicId);
        gl4duBindMatrix("modelMatrix");
        gl4duLoadIdentityf();

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(_pBasicId, "myTexture"), 0);
        glUniform1i(glGetUniformLocation(_pBasicId, "light"), 0);

        gl4duPushMatrix(); {
                static float _planeScale = 100.0f;
                gl4duRotatef(-90, 1, 0, 0);
                gl4duScalef(_planeScale, _planeScale, 1);
                gl4duSendMatrices();
        } gl4duPopMatrix();
        glBindTexture(GL_TEXTURE_2D, interact);
        gl4dgDraw(_plane);
        glUseProgram(0);
}

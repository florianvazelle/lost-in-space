#include "config.h"

void draw_template(GLuint _texId, float trans[3], float rotat[4], float scale[3]){
        glUseProgram(_pBasicId);

        glUniform1i(glGetUniformLocation(_pBasicId, "light"), 0);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        gl4duBindMatrix("projectionMatrix");
        gl4duPushMatrix(); {
                gl4duLoadIdentityf();
                gl4duBindMatrix("modelMatrix");
                gl4duPushMatrix(); {
                        gl4duLoadIdentityf();
                        gl4duTranslatef(trans[0], trans[1], trans[2]);
                        gl4duRotatef(rotat[0], rotat[1], rotat[2], rotat[3]);
                        gl4duScalef(scale[0], scale[1], scale[2]);
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

        glBindTexture(GL_TEXTURE_2D, _texId);
        gl4dgDraw(_plane);

        glDisable(GL_TEXTURE_2D);
        glBlendFunc(GL_ONE, GL_ZERO);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
}

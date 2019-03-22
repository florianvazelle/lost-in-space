#include <GL4D/gl4dp.h>
#include <SDL_image.h>

#include "satellite.h"
#include "utils/load_texture.h"

static GLuint _sphere = 0;

GLuint init_satellite(satellite s) {
        if (_sphere == 0)
                _sphere = gl4dgGenSpheref(50, 50);
        return load_2d_sprite_texture(s._texId, s.path_sprite);
}


void draw_satellite(satellite s, GLuint _pBasicId, GLuint _phong, GLfloat _eyePos[3]) {
        GLfloat _lumPos[4] = {250, 160, 30, 1.0};

        glUseProgram(_pBasicId);

        gl4duBindMatrix("modelMatrix");
        gl4duLoadIdentityf();

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(_pBasicId, "myTexture"), 0);

        glUniform1i(glGetUniformLocation(_pBasicId, "phong"), _phong);
        glUniform1i(glGetUniformLocation(_pBasicId, "type"), s.id);
        glUniform4fv(glGetUniformLocation(_pBasicId, "lumPos"), 1, _lumPos);

        gl4duPushMatrix(); {
                gl4duTranslatef(s.x, s.y, s.z);
                gl4duScalef(s.width, s.width, s.width);
                gl4duSendMatrices();
        } gl4duPopMatrix();

        glBindTexture(GL_TEXTURE_2D, s._texId);
        gl4dgDraw(_sphere);

        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
}

void quit_satellite(satellite s) {
        if (s._texId)
                glDeleteTextures(1, &s._texId);
}

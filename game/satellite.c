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

void draw_satellite(satellite s, GLuint _pBasicId) {
        glUseProgram(_pBasicId);
        gl4duBindMatrix("modelMatrix");
        gl4duLoadIdentityf();
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(_pBasicId, "tex"), 0);
        gl4duPushMatrix(); {
                gl4duTranslatef(s.x, s.y, s.z);
                gl4duScalef(s.width, s.width, s.width);
                gl4duSendMatrices();
        } gl4duPopMatrix();
        glBindTexture(GL_TEXTURE_2D, s._texId);
        glUniform1f(glGetUniformLocation(_pBasicId, "texRepeat"), 1.0);
        gl4dgDraw(_sphere);
        glUseProgram(0);
}

void quit_satellite(satellite s) {
        if (s._texId)
                glDeleteTextures(1, &s._texId);
}

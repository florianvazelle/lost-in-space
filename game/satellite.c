#include <GL4D/gl4dp.h>
#include <SDL_image.h>

#include "satellite.h"
#include "utils/load_texture.h"

static GLuint _sphere = 0;

static float inertia = 0.0f;
static GLfloat _lumPos[4] = {250, 160, 30, 1.0};

GLuint init_satellite(satellite s) {
        if (_sphere == 0)
                _sphere = gl4dgGenSpheref(50, 50);
        return load_2d_sprite_texture(s._texId, s.path_sprite);
}

vector3 ellipse(satellite s){
        s.dir.x = 250 + 178 * cos(2 * M_PI / 360 * inertia) - s.data.x;
        s.dir.y = 160 + 178 * sin(2 * M_PI / 360 * inertia) - s.data.y;
        s.dir.z = 30  + 178 * sin(2 * M_PI / 360 * inertia) - s.data.z;
        inertia += 0.2f;
        if(inertia >= 360.0f) inertia = 0.0f;
        return s.dir;
}

vector3 update_satellite(satellite s) {
        if(s.id == 2) {
                s.dir = ellipse(s);
        } else {
                s.dir = zero();
        }
        return s.dir;
}

void draw_satellite(satellite s, GLuint _pBasicId, GLuint _phong) {
        glUseProgram(_pBasicId);

        gl4duBindMatrix("modelMatrix");
        gl4duLoadIdentityf();

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(_pBasicId, "myTexture"), 0);

        glUniform1i(glGetUniformLocation(_pBasicId, "phong"), _phong);
        glUniform1i(glGetUniformLocation(_pBasicId, "type"), s.id);
        glUniform4fv(glGetUniformLocation(_pBasicId, "lumPos"), 1, _lumPos);

        gl4duPushMatrix(); {
                gl4duTranslatef(s.data.x, s.data.y, s.data.z);
                glRotatef(180, 0, 1, 0);
                gl4duScalef(s.data.rayon, s.data.rayon, s.data.rayon);
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

int hit_satellite(satellite s1, satellite s2){
        sphere s1_fut = apply_dir(s1.data, s1.dir);
        sphere s2_fut = apply_dir(s2.data, s2.dir);
        return hit_sphere_sphere(s1_fut, s2_fut);
}

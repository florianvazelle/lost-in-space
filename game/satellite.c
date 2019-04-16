#include <GL4D/gl4dp.h>
#include <SDL_image.h>

#include "satellite.h"
#include "config.h"
#include "utils/load_texture.h"

static float rot_earth = 0.0f;
static GLfloat _lumPos[4] = {250, 160, 30, 1.0};

GLuint init_satellite(satellite s) {
        return load_2d_sprite_texture(s._texId, s.path_sprite);
}

static void ellipse(satellite* s){
        s->body.dir.x = 250 + 200 * cos(2 * M_PI / 360 * rot_earth) - s->body.data.x;
        s->body.dir.y = 160 + 200 * sin(2 * M_PI / 360 * rot_earth) - s->body.data.y;
        s->body.dir.z = 30  + 200 * sin(2 * M_PI / 360 * rot_earth) - s->body.data.z;
        //printf("%.03f %.03f %.03f\n", s->body.dir.x, s->body.dir.y, s->body.dir.z);
        rot_earth += 0.2f;
        if(rot_earth >= 360.0f) rot_earth = 0.0f;
}

int update_satellite(satellite* s) {
        /* Mouvement */
        if(s->id == 2) {
                ellipse(s);
        } else if(s->id == 3) {
                s->body.dir = inertia();
        } else {
                s->body.dir = zero();
        }
        /* Collision */
        if(s->collision == 1) s->body.dir = invert(s->body.dir);
        /* Explosion */
        if(s->explosion > 0.0) {
                s->explosion += 0.01;
                if(s->explosion > 1.0) return 1;
        }
        return 0;
}

void draw_satellite(satellite s) {
        glUseProgram(_pBasicId);

        gl4duBindMatrix("modelMatrix");
        gl4duLoadIdentityf();

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(_pBasicId, "myTexture"), 0);

        glUniform1i(glGetUniformLocation(_pBasicId, "light"), _light);
        glUniform1i(glGetUniformLocation(_pBasicId, "type"), s.id);
        glUniform1f(glGetUniformLocation(_pBasicId, "explosion"), s.explosion);
        glUniform4fv(glGetUniformLocation(_pBasicId, "lumPos"), 1, _lumPos);

        gl4duPushMatrix(); {
                gl4duTranslatef(s.body.data.x, s.body.data.y, s.body.data.z);
                glRotatef(180, 0, 1, 0);
                gl4duScalef(s.body.data.rayon, s.body.data.rayon, s.body.data.rayon);
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
        sphere s1_fut = apply_dir(s1.body.data, s1.body.dir);
        sphere s2_fut = apply_dir(s2.body.data, s2.body.dir);
        return hit_sphere_sphere(s1_fut, s2_fut);
}

void explose(satellite* s){
        s->explosion = 0.01;
        //s->body.data.rayon = 0;
}

#include <GL4D/gl4dp.h>

#include "config.h"
#include "util/template.h"
#include "util/load_texture.h"

static GLuint _cockpitTexId = 0;

void init_cockpit() {
        load_2d_sprite_texture(&_cockpitTexId, "assets/textures/cockpit.png");
}

void draw_cockpit() {
        float trans[3] = {0.0, 0.0, 0.0};
        float rotat[4] = {180, 0, 0, 1};
        float scale[3] = {1, 1, 1};
        draw_template(_cockpitTexId, trans, rotat, scale);
}

void quit_cockpit() {
        if (_cockpitTexId)
                glDeleteTextures(1, &_cockpitTexId);
}

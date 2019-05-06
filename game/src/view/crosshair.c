#include <GL4D/gl4dp.h>

#include "config.h"
#include "util/template.h"
#include "util/load_texture.h"

static GLuint _crosshairTexId = 0;

void init_crosshair() {
        GLuint _crosshairTex[] = {RGB(255, 255, 255)};
        load_2d_texture(&_crosshairTexId, _crosshairTex);
}

void draw_crosshair(float x, float y) {
        float trans[3] = {-0.1 + x, 0.05 + y, 0.0};
        float rotat[4] = {60, 0, 0, 1};
        float scale[3] = {0.01 / 5.0, 0.2 / 5.0, 0.2 / 5.0};
        draw_template(_crosshairTexId, trans, rotat, scale);

        trans[1] = -0.05 + y;
        rotat[0] = 120;
        draw_template(_crosshairTexId, trans, rotat, scale);

        trans[0] = 0.1 + x;
        rotat[0] = -120;
        draw_template(_crosshairTexId, trans, rotat, scale);

        trans[1] = 0.05 + y;
        rotat[0] = -60;
        draw_template(_crosshairTexId, trans, rotat, scale);
}

void quit_crosshair() {
        if (_crosshairTexId)
                glDeleteTextures(1, &_crosshairTexId);
}

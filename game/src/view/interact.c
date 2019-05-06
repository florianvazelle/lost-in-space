#include <GL4D/gl4dp.h>
#include <SDL_image.h>

#include "config.h"
#include "util/template.h"
#include "util/load_texture.h"

static GLuint _interactTexId = 0;

void init_interact() {
        load_2d_sprite_texture(&_interactTexId, "assets/textures/entree.png");
}

void draw_interact() {
        float trans[3] = {0.75, 0.75, 0.0};
        float rotat[4] = {180, 1, 0, 0};
        float scale[3] = {0.2, 0.25, 0.2};
        draw_template(_interactTexId, trans, rotat, scale);
}

void quit_interact() {
        if (_interactTexId)
                glDeleteTextures(1, &_interactTexId);
}

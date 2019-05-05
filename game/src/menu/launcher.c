#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>

#include "config.h"
#include "menu/button.h"
#include "action/hypervelocity.h"

void init_launcher(int argc, char **argv) {
        init_button();
}

void mousepressed_launcher(int button, int state, int x, int y) {
        if (button == 1 && state == 1) {
                float nx = ((float)x / (float)_wW) * 2.0f - 1.0f;
                float ny = 1.0f - ((float)y / (float)_wH) * 2.0f;
                //printf("%d %d %.02f %.02f\n", x, y, nx, ny);
                TestAll(nx, ny);
        }
}

void draw_launcher() {
        animate_hypervelocity();
        DrawButton();
}

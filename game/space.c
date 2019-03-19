#include "satellite.h"
#include "skybox.h"
#include "../libs/sort/tri_fusion.h"

#define LENGTH 3

static satellite s[] = {
        {0, 0, 0, 100, "planet", "assets/textures/terre.png", 0},
        {200, 0, 0, 10, "asteroid", "assets/textures/asteroid.jpg", 0},
        {0, 200, 0, 10, "asteroid", "assets/textures/asteroid.jpg", 0}
};

void init_space(){
        for(int i = 0; i < LENGTH; i++) {
                s[i]._texId = init_satellite(s[i]);
        }
        init_skybox();
}

void update_space(int x, int y, int z){
        tri_fusion(s, LENGTH, x, y, z);
}

void draw_space(GLuint _pBasicId){
        for (int i = LENGTH - 1; i >= 0; i--) {
                draw_satellite(s[i], _pBasicId);
        }
        draw_skybox();
}

void quit_space(){
        for(int i = 0; i < LENGTH; i++) {
                quit_satellite(s[i]);
        }
        quit_skybox();
}

#include <time.h>
#include <stdlib.h>

#include "satellite.h"
#include "utils/merge.h"

static satellite s[4] = {
        {1, { 428, 160, 30, 100 }, { 0, 0, 0 }, "assets/textures/star.jpg", 0},
        {2, { 150, -124, 170, 78 }, { 0, 0, 0 }, "assets/textures/terre.png", 0},
        {3, { 141, 78, 60, 12 }, { 0, 0, 0 }, "assets/textures/asteroid.jpg", 0},
        {3, { 91, 40, 230, 5 }, { 0, 0, 0 }, "assets/textures/asteroid.jpg", 0}
};

static int size = 4;

void init_space(){
        for(int i = 0; i < size; i++) {
                s[i]._texId = init_satellite(s[i]);
        }
}

void update_space(int x, int y, int z) {
        /* Calcul les vecteurs de directions */
        for(int i = 0; i < size; i++) {
                s[i].dir = update_satellite(s[i]);
        }

        /* Test si il a collision entre les satellites */
        for(int i = 0; i < size; i++) {
                for(int j = 0; j < size; j++) {
                        if(i == j) continue;
                        if(hit_satellite(s[i], s[j]) == 1) {
                                /* Si collision alors on modifie les vecteurs de direction */
                                //s[i].dir = invert(s[i].dir);
                                //s[j].dir = invert(s[j].dir);
                        }
                }
        }

        /* Applique le vecteur de direction */
        for(int i = 0; i < size; i++) {
                s[i].data = apply_dir(s[i].data, s[i].dir);
        }

        /* Trie les planetes en fonction de leur distance */
        tri_fusion(s, size, x, y, z);
}

void draw_space(GLuint _pBasicId, GLuint _phong){
        for (int i = size - 1; i >= 0; i--) {
                draw_satellite(s[i], _pBasicId, _phong);
        }
}

void apply_stars(GLuint _pBasicId){
        for (int i = size - 1; i >= 0; i--) {
                if(s[i].id == 1) {
                        GLfloat _lumPos[4] = {s[i].data.x, s[i].data.y, s[i].data.z, 1.0};
                        glUniform4fv(glGetUniformLocation(_pBasicId, "lumPos"), 1, _lumPos);
                        return;
                }
        }
}

void quit_space(){
        for(int i = 0; i < size; i++) {
                quit_satellite(s[i]);
        }
}

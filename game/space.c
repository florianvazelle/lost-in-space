#include "satellite.h"
#include "utils/merge.h"

static satellite s[4] = {
        {1, { { 250, 160, 30, 100 }, { 0, 0, 0 } }, "assets/textures/star.jpg", 0, 0},
        {2, { { 150, -124, 170, 78 }, { 0, 0, 0 } }, "assets/textures/terre.png", 0, 0},
        {3, { { 141, 78, 60, 12 }, { 0, 0, 0 } }, "assets/textures/asteroid.jpg", 0, 0},
        {3, { { 91, 40, 230, 5 }, { 0, 0, 0 } }, "assets/textures/asteroid.jpg", 0, 0}
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
                s[i].body.dir = update_satellite(s[i]);
        }

        /* Test si il a collision entre les satellites */
        for(int i = 0; i < size; i++) {
                for(int j = 0; j < size; j++) {
                        if(i == j) continue;
                        if(hit_satellite(s[i], s[j]) == 1) {
                                /* Si collision alors on modifie les vecteurs de direction */
                                s[i].collision = !s[i].collision;
                                s[j].collision = !s[j].collision;
                        }
                }
        }

        /* Applique le vecteur de direction */
        for(int i = 0; i < size; i++) {
                s[i].body.data = apply_dir(s[i].body.data, s[i].body.dir);
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
                        GLfloat _lumPos[4] = {s[i].body.data.x, s[i].body.data.y, s[i].body.data.z, 1.0};
                        glUniform4fv(glGetUniformLocation(_pBasicId, "lumPos"), 1, _lumPos);
                        return;
                }
        }
}

GLuint hit_player_satellite(entitie player){
        sphere player_fut = apply_dir(player.data, player.dir);
        for(int i = 0; i < size; i++) {
                if(hit_sphere_sphere(player_fut, s[i].body.data) == 1) {
                        return s[i]._texId;
                }
        }
        return 0;
}

void quit_space(){
        for(int i = 0; i < size; i++) {
                quit_satellite(s[i]);
        }
}

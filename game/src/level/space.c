#include "config.h"
#include "level/satellite.h"
#include "util/merge.h"

static satellite s[4] = {
        {1, {{250, 160, 30, 100}, {0, 0, 0}}, "assets/textures/star.jpg", 0, 0, 0.0},
        {2, {{150, -124, 170, 78}, {0, 0, 0}}, "assets/textures/terre.png", 0, 0, 0.0},
        {3, {{141, 78, 60, 12}, {0, 0, 0}}, "assets/textures/asteroid.jpg", 0, 0, 0.0},
        {3, {{91, 40, 230, 5}, {0, 0, 0}}, "assets/textures/asteroid.jpg", 0, 0, 0.0}
};

static int size = 4;

void init_space() {
        /* Initialisation des differents satellites
           (Chargement des textures) */
        for (int i = 0; i < size; i++) {
                init_satellite(&s[i]);
        }
}

void update_space(int x, int y, int z) {
        /* Calcul les vecteurs de directions */
        for (int i = 0; i < size; i++) {
                int call_back = update_satellite(&s[i]);
                if (call_back == 1) {
                        printf("Fin\n");
                        for (int j = i; j < size - 1; j++)
                                s[j] = s[j + 1];
                        size -= 1;
                }
        }

        /* Test si il a collision entre les satellites */
        for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                        if (i == j)
                                continue;
                        if (hit_satellite(s[i], s[j]) == 1) {
                                /* Si collision alors on modifie les vecteurs de direction */
                                s[i].collision = !s[i].collision;
                                s[j].collision = !s[j].collision;
                        }
                }
        }

        /* Applique le vecteur de direction */
        for (int i = 0; i < size; i++) {
                s[i].body.data = apply_dir(s[i].body.data, s[i].body.dir);
        }

        /* Trie les planetes en fonction de leur distance */
        tri_fusion(s, size, x, y, z);
}

void draw_space() {
        /* Affichage des differents satellites */
        for (int i = size - 1; i >= 0; i--) {
                draw_satellite(s[i]);
        }
}

void apply_stars(GLuint _pId) {
        /* Application de la lumiere en fonction du soleil le plus proche */
        for (int i = size - 1; i >= 0; i--) {
                if (s[i].id == 1) {
                        GLfloat _lumPos[4] = {s[i].body.data.x, s[i].body.data.y,
                                              s[i].body.data.z, 1.0};
                        glUniform4fv(glGetUniformLocation(_pId, "lumPos"), 1, _lumPos);
                        break;
                }
        }
}

int hit_player_satellite(entitie player) {
        /* Test la collision entre le joueur et les satellites */
        sphere player_fut = apply_dir(player.data, player.dir);
        for (int i = 0; i < size; i++) {
                if (hit_sphere_sphere(player_fut, s[i].body.data) == 1) {
                        return 1;
                }
        }
        return 0;
}

GLuint hit_interact(sphere player) {
        /* Test si le joueur est dans une zone d'interaction */
        for (int i = 0; i < size; i++) {
                if (hit_sphere_sphere(player, s[i].body.data) == 1) {
                        return s[i]._texId;
                }
        }
        return 0;
}

void hit_shoot_satellite(vector3 player, vector3 shoot) {
        for (int i = 0; i < size; i++) {
                if (hit_line_sphere(player, shoot, s[i].body.data) == 1) {
                        printf("PAN !!\n");
                        explose(&s[i]);
                }
        }
}

void quit_space() {
        for (int i = 0; i < size; i++) {
                quit_satellite(s[i]);
        }
}

// debug
void explose_test() {
        explose(&s[0]);
}

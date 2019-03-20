#include <time.h>
#include <stdlib.h>

#include "satellite.h"
#include "utils/merge.h"

static satellite s[4] = {
        {1, 250, 160, 30, 100, "assets/textures/star.jpg", 0},
        {2, 150, -124, 170, 78, "assets/textures/terre.png", 0},
        {3, 141, 78, 60, 12, "assets/textures/asteroid.jpg", 0},
        {3, 91, 40, 230, 5, "assets/textures/asteroid.jpg", 0}
};
static int size = 4;
static float inertia = 0.0f;

int myrandom(int min, int max){
        srand(time(NULL));
        return rand() % max + min;
}

void create_satellite_info(){
        int nb_stars = 3; //random(1, 3);
        int nb_planets = 5; //random(5, 7);
        int nb_asteroids = 15; //random(10, 15);

        //s = realloc(s, sizeof(satellite) * (size + nb_stars));
        for(int i = 0; i < nb_stars; i++) {
                int x = myrandom(50, 100);
                int y = myrandom(50, 100);
                int z = myrandom(50, 100);
                int w = myrandom(80, 120);
                satellite new = {1, x, y, z, w, "assets/textures/star.jpg", 0};
                s[size++] = new;
        }

        //s = realloc(s, sizeof(satellite) * (size + nb_planets));
        for(int i = 0; i < nb_planets; i++) {
                int x = myrandom(50, 100);
                int y = myrandom(50, 100);
                int z = myrandom(50, 100);
                int w = myrandom(80, 120);
                satellite new = {2, x, y, z, w, "assets/textures/terre.png", 0};
                s[size++] = new;
        }

        //s = realloc(s, sizeof(satellite) * (size + nb_asteroids));
        for(int i = 0; i < nb_asteroids; i++) {
                int x = myrandom(50, 100);
                int y = myrandom(50, 100);
                int z = myrandom(50, 100);
                int w = myrandom  (5, 15);
                satellite new = {3, x, y, z, w, "assets/textures/asteroid.jpg", 0};
                s[size++] = new;
        }
}

void init_space(){
        //create_satellite_info();

        for(int i = 0; i < size; i++) {
                s[i]._texId = init_satellite(s[i]);
        }
}

void ellipse(double dt){
        for(int i = 0; i< size; i++) {
                if(s[i].id == 2) {
                        s[i].x = 250 + 178 * cos(2 * M_PI / 360 * inertia);
                        s[i].y = 160 + 178 * sin(2 * M_PI / 360 * inertia);
                        s[i].z = 30  + 178 * sin(2 * M_PI / 360 * inertia);
                        inertia += 0.2f;
                        if(inertia >= 360.0f) inertia = 0.0f;
                }
        }
}

void update_space(int x, int y, int z, double dt){
        tri_fusion(s, size, x, y, z);
        ellipse(dt);
}

void draw_space(GLuint _pBasicId){
        for (int i = size - 1; i >= 0; i--) {
                draw_satellite(s[i], _pBasicId);
        }
}

void quit_space(){
        for(int i = 0; i < size; i++) {
                quit_satellite(s[i]);
        }
}

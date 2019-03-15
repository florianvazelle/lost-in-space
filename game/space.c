#include "planet.h"
#include "asteroid.h"

planet terre = {0, 0, 0, 100, "../assets/textures/terre.png"};
asteroid a[] = {
        {200, 0, 0, 10, "../assets/textures/asteroid.jpg"},
        {0, 200, 0, 10, "../assets/textures/asteroid.jpg"}
};

void init_space(){
        init_planet(terre);
        init_asteroid(a[0]);
}

void draw_space(){
        draw_planet(terre);
        for(int i = 0; i < 2; i++)
                draw_asteroid(a[i]);
}

void quit_space(){
        quit_planet();
        quit_asteroid();
}

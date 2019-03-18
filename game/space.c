#include "satellite.h"
#include "../libs/sort/tri_fusion.h"

#define LENGTH 3

static satellite s[] = {
        {0, 0, 0, 100, "planet", "assets/textures/terre.png", 0},
        {200, 0, 0, 10, "asteroid", "assets/textures/asteroid.jpg", 0},
        {0, 200, 0, 10, "asteroid", "assets/textures/asteroid.jpg", 0}
};

static GLuint _plane = 0;
static GLuint _pSkyboxId = 0;
static GLuint _spaceTexId = 0;

static GLuint vbo_cube_vertices = 0;
static GLuint ibo_cube_indices = 0;
static GLushort cube_indices[] = {
        0, 1, 2, 3,
        3, 2, 6, 7,
        7, 6, 5, 4,
        4, 5, 1, 0,
        0, 3, 7, 4,
        1, 2, 6, 5
};

void draw_galaxy_skybox();

void setupCubeMap2() {
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_CUBE_MAP);
        glGenTextures(1, &_spaceTexId);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _spaceTexId);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void setupCubeMap(SDL_Surface *xpos, SDL_Surface *xneg, SDL_Surface *ypos, SDL_Surface *yneg, SDL_Surface *zpos, SDL_Surface *zneg) {
        setupCubeMap2(_spaceTexId);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos->w, xpos->h, 0, xpos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, xpos->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg->w, xneg->h, 0, xneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, xneg->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos->w, ypos->h, 0, ypos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, ypos->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg->w, yneg->h, 0, yneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, yneg->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos->w, zpos->h, 0, zpos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, zpos->pixels);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg->w, zneg->h, 0, zneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, zneg->pixels);
}

void init_space(){
        for(int i = 0; i < LENGTH; i++) {
                s[i]._texId = init_satellite(s[i]);
        }

        _pSkyboxId = gl4duCreateProgram("<vs>game/shaders/skybox.vs", "<fs>game/shaders/skybox.fs", NULL);

        _plane = gl4dgGenQuadf();
        SDL_Surface *xpos = IMG_Load("assets/textures/skybox_front.png"); SDL_Surface *xneg = IMG_Load("assets/textures/skybox_back.png");
        SDL_Surface *ypos = IMG_Load("assets/textures/skybox_top.png"); SDL_Surface *yneg = IMG_Load("assets/textures/skybox_bottom.png");
        SDL_Surface *zpos = IMG_Load("assets/textures/skybox_rigth.png"); SDL_Surface *zneg = IMG_Load("assets/textures/skybox_left.png");
        setupCubeMap(xpos, xneg, ypos, yneg, zpos, zneg);
        SDL_FreeSurface(xneg);  SDL_FreeSurface(xpos);
        SDL_FreeSurface(yneg);  SDL_FreeSurface(ypos);
        SDL_FreeSurface(zneg);  SDL_FreeSurface(zpos);

        GLint vertex = glGetAttribLocation(_pSkyboxId, "vertex");

        static GLfloat cube_vertices[] = {
                -1.0,  1.0,  1.0,
                -1.0, -1.0,  1.0,
                1.0, -1.0,  1.0,
                1.0,  1.0,  1.0,
                -1.0,  1.0, -1.0,
                -1.0, -1.0, -1.0,
                1.0, -1.0, -1.0,
                1.0,  1.0, -1.0
        };

        glGenBuffers(1, &vbo_cube_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(vertex);
        glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &ibo_cube_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void update_space(int x, int y, int z){
        tri_fusion(s, LENGTH, x, y, z);
}

void draw_space(GLuint _pBasicId){
        draw_galaxy_skybox();
        for (int i = LENGTH - 1; i >= 0; i--) {
                draw_satellite(s[i], _pBasicId);
        }
}

void quit_space(){
        for(int i = 0; i < LENGTH; i++) {
                quit_satellite(s[i]);
        }
        glDeleteTextures(1, &_spaceTexId);
        if(vbo_cube_vertices)
                glDeleteBuffers(1, &vbo_cube_vertices);
        if(ibo_cube_indices)
                glDeleteBuffers(1, &ibo_cube_indices);
}

void draw_galaxy_skybox() {
        glEnable(GL_TEXTURE_CUBE_MAP);
        glUseProgram(_pSkyboxId);

        gl4duBindMatrix("modelMatrix");
        gl4duLoadIdentityf();

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _spaceTexId);
        glUniform1i(glGetUniformLocation(_pSkyboxId, "tex"), 0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_indices);

        gl4duSendMatrices();
        glDrawElements(GL_QUADS, sizeof(cube_indices)/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glUseProgram(0);
}

#include <GL4D/gl4dp.h>

#include "util/load_texture.h"

static GLuint _pSkyboxId = 0;
static GLuint _spaceTexId = 0;

static GLuint skyboxVAO = 0;

void init_skybox() {
        /* Chargement des shaders pour la skybox */
        _pSkyboxId = gl4duCreateProgram("<vs>assets/shaders/skybox.vs",
                                        "<fs>assets/shaders/skybox.fs", NULL);

        /* Chargement de la texture cube */
        const char *skybox_path_sprite[6] = {
                "assets/textures/skybox/rigth.png", "assets/textures/skybox/left.png",
                "assets/textures/skybox/top.png",   "assets/textures/skybox/bottom.png",
                "assets/textures/skybox/front.png", "assets/textures/skybox/back.png"
        };
        load_cube_map_sprite_texture(&_spaceTexId, skybox_path_sprite);

        GLfloat skyboxVertices[] = {
                -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

                -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
                -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

                1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

                -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

                -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

                -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
                1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f
        };

        GLuint skyboxVBO = 0;

        /* Configuration de la skybox VAO */
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
        glBindVertexArray(0);
}

void draw_skybox(GLfloat x, GLfloat y, GLfloat z) {
        glDepthFunc(GL_LEQUAL); // Change depth function so depth test passes when
                                // values are equal to depth buffer's content
        glUseProgram(_pSkyboxId);

        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _spaceTexId);
        gl4duPushMatrix(); {
                gl4duTranslatef(x, y, z);
                gl4duSendMatrices();
        } gl4duPopMatrix();

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // Set depth function back to default

        glUseProgram(0);
}

void quit_skybox() {
        if (_spaceTexId)
                glDeleteTextures(1, &_spaceTexId);
}

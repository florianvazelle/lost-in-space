#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>

#include "config.h"
#include "util/load_text.h"
#include "action/hypervelocity.h"

static GLuint _textTexId = 0;
static GLfloat t0 = -1;

void init_credit() {
        initText(&_textTexId,
                 "               Lost In Space\n\n"
                 "    On an original idea of Florian Vazelle\n\n"
                 "       Developped by Florian Vazelle\n"
                 "       Font designed by Tension Type\n"
                 "      Someone is missing for music ?!\n"
                 "           3D model by javier8\n"
                 "\n           Thanks for playing <3");
}

void reinit_credit(){
        t0 = -1;
}

void draw_credit() {
        animate_hypervelocity();

        const GLfloat inclinaison = -45.0;
        GLfloat t, d;

        if(t0 < 0.0f)
                t0 = SDL_GetTicks();
        t = (SDL_GetTicks() - t0) / 1000.0f, d = -1.5f /* du retard pour commencer en bas */ + 0.1f /* vitesse */ * t;

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUseProgram(_pTextId);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _textTexId);

        glUniform1i(glGetUniformLocation(_pTextId, "myTexture"), 0);

        gl4duBindMatrix("modelMatrix");
        gl4duLoadIdentityf();
        gl4duScalef(2, 4, 1);
        gl4duTranslatef(0,  d * cos(inclinaison * M_PI / 180.0f), -4 + d * sin(inclinaison * M_PI / 180.0f));
        gl4duRotatef(inclinaison, 1, 0, 0);
        gl4duSendMatrices();

        gl4dgDraw(_plane);

        glUseProgram(0);
}

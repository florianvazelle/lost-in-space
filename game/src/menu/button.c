#include <GL4D/gl4dp.h>
#include <GL4D/gl4du.h>
#include <GL4D/gl4duw_SDL2.h>
#include <SDL_image.h>

#include "config.h"
#include "util/struct/vector3.h"
#include "util/convert.h"
#include "util/load_text.h"

typedef void (*ButtonCallback)();

typedef struct {
        float x, y, w, h;
        char *label;
        GLuint textId;
        ButtonCallback callbackFunction;
} Button;

static void TheButtonCallback() {
        printf("I have been called\n");
}

static void PlayCallback() {
        _state = PLAY;
}

static void SettingsCallback() {
        //_state = SETTINGS;
}

static void CreditsCallback() {
        _state = CREDITS;
}

static void QuitCallback() {
        exit(0);
}

static Button MyButton[5] = {
        {0, 0.5, 0.5, 0.25, "Lost In Space", 0, TheButtonCallback},
        {-0.5, -0.25, 0.25, 0.1, "Play", 0, PlayCallback},
        {-0.5, -0.5, 0.25, 0.1, "Quit", 0, QuitCallback},
        {0.45, -0.25, 0.25, 0.1, "Settings", 0, SettingsCallback},
        {0.45, -0.5, 0.25, 0.1, "Credits", 0, CreditsCallback}
};
static int size = 5;

static int ButtonClickTest(Button *box, float x, float y) {
        if (x >= (box->x - box->w) && x < (box->x + box->w)
            && y >= (box->y - box->h) && y < (box->y + box->h))
                return 1;
        return 0;
}

static void ButtonRelease(Button *b, float x, float y) {
        if (ButtonClickTest(b, x, y) == 1) {
                b->callbackFunction();
        }
}

void TestAll(float xpress, float ypress) {
        for (int i = 0; i < size; i++) {
                ButtonRelease(&MyButton[i], xpress, ypress);
        }
}

void init_button() {
        for (int i = 0; i < size; i++) {
                initText(&MyButton[i].textId, MyButton[i].label);
        }
}

void DrawButton() {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glUseProgram(_pTextId);

        for (int i = 0; i < size; i++) {

                Button currentButton = MyButton[i];

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, currentButton.textId);
                glUniform1i(glGetUniformLocation(_pTextId, "myTexture"), 0);

                float mouse[2] = {_xm, _ym};
                sdl2gl(mouse);

                if (i == 0 || ButtonClickTest(&currentButton, mouse[0], mouse[1]) == 1) {
                        glUniform1i(glGetUniformLocation(_pTextId, "highlight"), 1);
                } else {
                        glUniform1i(glGetUniformLocation(_pTextId, "highlight"), 0);
                }

                gl4duBindMatrix("projectionMatrix");
                gl4duPushMatrix(); {
                        gl4duLoadIdentityf();
                        gl4duBindMatrix("modelMatrix");
                        gl4duPushMatrix(); {
                                gl4duLoadIdentityf();
                                gl4duTranslatef(currentButton.x, currentButton.y, 0);
                                gl4duScalef(currentButton.w, currentButton.h, 0);
                                gl4duSendMatrices();
                        } gl4duPopMatrix();
                        gl4duBindMatrix("projectionMatrix");
                } gl4duPopMatrix();
                gl4duBindMatrix("modelMatrix");

                gl4dgDraw(_plane);
        }

        glBlendFunc(GL_ONE, GL_ZERO);
        glDisable(GL_BLEND);
        glBindTexture(GL_TEXTURE_2D, 0);
        glUseProgram(0);
}

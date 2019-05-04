#include <GL4D/gl4dp.h>
#include <SDL_image.h>

#include "config.h"
#include "util/load_texture.h"

static GLuint _interactTexId = 0;

void init_interact() {
  load_2d_sprite_texture(&_interactTexId, "assets/textures/entree.png");
}

void draw_interact() {
  glUseProgram(_pBasicId);

  glUniform1i(glGetUniformLocation(_pBasicId, "light"), 0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  gl4duBindMatrix("projectionMatrix");
  gl4duPushMatrix();
  {
    gl4duLoadIdentityf();
    gl4duBindMatrix("modelMatrix");
    gl4duPushMatrix();
    {
      gl4duLoadIdentityf();
      gl4duTranslatef(0.75, 0.75, 0.0);
      gl4duRotatef(180, 1, 0, 0);
      gl4duScalef(0.2, 0.25, 0.2);
      gl4duBindMatrix("viewMatrix");
      gl4duPushMatrix();
      {
        gl4duLoadIdentityf();
        gl4duSendMatrices();
      }
      gl4duPopMatrix();
      gl4duBindMatrix("modelMatrix");
    }
    gl4duPopMatrix();
    gl4duBindMatrix("projectionMatrix");
  }
  gl4duPopMatrix();
  gl4duBindMatrix("modelMatrix");

  glBindTexture(GL_TEXTURE_2D, _interactTexId);
  gl4dgDraw(_plane);

  glDisable(GL_TEXTURE_2D);
  glBlendFunc(GL_ONE, GL_ZERO);
  glDisable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
}

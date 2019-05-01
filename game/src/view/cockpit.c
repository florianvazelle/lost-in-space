#include <GL4D/gl4dp.h>

#include "config.h"
#include "util/load_texture.h"

static GLuint _cockpitTexId = 0;

void init_cockpit() {
  _cockpitTexId =
      load_2d_sprite_texture(_cockpitTexId, "assets/textures/cockpit.png");
}

void draw_cockpit() {
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
      gl4duRotatef(180, 0, 0, 1);
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

  glBindTexture(GL_TEXTURE_2D, _cockpitTexId);
  gl4dgDraw(_plane);

  glDisable(GL_TEXTURE_2D);
  glBlendFunc(GL_ONE, GL_ZERO);
  glDisable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, 0);
  glUseProgram(0);
}

void quit_cockpit() {
  if (_cockpitTexId)
    glDeleteTextures(1, &_cockpitTexId);
}

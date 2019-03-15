#ifndef _CROSSHAIR_H
#define _CROSSHAIR_H

#include <GL4D/gl4dp.h>

static GLuint _plane = 0;
static GLuint _crosshairTexId = 0;

void init_crosshair() {
  _plane = gl4dgGenQuadf();

  GLuint _crosshiarTex[] = {RGB(255, 255, 255)};
  glGenTextures(1, &_crosshairTexId);
  glBindTexture(GL_TEXTURE_2D, _crosshairTexId);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               _crosshiarTex);
}

static void part_crosshair(double rot, double posx, double posy) {
  gl4duBindMatrix("projectionMatrix");
  gl4duPushMatrix();
  {
    gl4duLoadIdentityf();
    gl4duBindMatrix("modelMatrix");
    gl4duPushMatrix();
    {
      gl4duLoadIdentityf();
      gl4duTranslatef(posx, posy, 0.0);
      gl4duRotatef(rot, 0, 0, 1);
      gl4duScalef(0.01 / 5.0, 0.2 / 5.0, 0.2 / 5.0);
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

  gl4dgDraw(_plane);
}

void draw_crosshair(float x, float y) {
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, _crosshairTexId);

  part_crosshair(60.0, -0.1 + x, 0.05 + y);
  part_crosshair(120.0, -0.1 + x, -0.05 + y);
  part_crosshair(-60.0, 0.1 + x, 0.05 + y);
  part_crosshair(-120.0, 0.1 + x, -0.05 + y);
}

void quit_crosshair() {
  if (_crosshairTexId)
    glDeleteTextures(1, &_crosshairTexId);
}

#endif

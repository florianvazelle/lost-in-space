static GLuint _pSkyboxId = 0;
static GLuint _spaceTexId = 0;

static GLuint skyboxVAO, skyboxVBO;

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

void setupCubeMap(SDL_Surface *xpos, SDL_Surface *xneg, SDL_Surface *ypos,
                  SDL_Surface *yneg, SDL_Surface *zpos, SDL_Surface *zneg) {
  setupCubeMap2(_spaceTexId);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, xpos->w, xpos->h, 0,
               xpos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
               GL_UNSIGNED_BYTE, xpos->pixels);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, xneg->w, xneg->h, 0,
               xneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
               GL_UNSIGNED_BYTE, xneg->pixels);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, ypos->w, ypos->h, 0,
               ypos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
               GL_UNSIGNED_BYTE, ypos->pixels);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, yneg->w, yneg->h, 0,
               yneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
               GL_UNSIGNED_BYTE, yneg->pixels);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, zpos->w, zpos->h, 0,
               zpos->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
               GL_UNSIGNED_BYTE, zpos->pixels);
  glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, zneg->w, zneg->h, 0,
               zneg->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
               GL_UNSIGNED_BYTE, zneg->pixels);
}

void init_skybox() {
  _pSkyboxId = gl4duCreateProgram("<vs>game/shaders/skybox.vs",
                                  "<fs>game/shaders/skybox.fs", NULL);

  SDL_Surface *xpos = IMG_Load("assets/textures/skybox_rigth.png");
  SDL_Surface *xneg = IMG_Load("assets/textures/skybox_left.png");
  SDL_Surface *ypos = IMG_Load("assets/textures/skybox_top.png");
  SDL_Surface *yneg = IMG_Load("assets/textures/skybox_bottom.png");
  SDL_Surface *zpos = IMG_Load("assets/textures/skybox_front.png");
  SDL_Surface *zneg = IMG_Load("assets/textures/skybox_back.png");
  setupCubeMap(xpos, xneg, ypos, yneg, zpos, zneg);
  SDL_FreeSurface(xneg);
  SDL_FreeSurface(xpos);
  SDL_FreeSurface(yneg);
  SDL_FreeSurface(ypos);
  SDL_FreeSurface(zneg);
  SDL_FreeSurface(zpos);

  GLfloat skyboxVertices[] = {
      // Positions
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
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  // Setup skybox VAO
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                        (GLvoid *)0);
  glBindVertexArray(0);
}

void draw_skybox(GLfloat x, GLfloat y, GLfloat z) {
  // Draw skybox as last
  glDepthFunc(GL_LEQUAL); // Change depth function so depth test passes when
                          // values are equal to depth buffer's content
  glUseProgram(_pSkyboxId);
  glActiveTexture(GL_TEXTURE0);

  // skybox cube
  glBindVertexArray(skyboxVAO);
  glBindTexture(GL_TEXTURE_CUBE_MAP, _spaceTexId);
  gl4duPushMatrix();
  {
    gl4duTranslatef(x, y, z);
    gl4duSendMatrices();
  }
  gl4duPopMatrix();
  // gl4duSendMatrices();

  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // Set depth function back to default

  glUseProgram(0);
}

void quit_skybox() { glDeleteTextures(1, &_spaceTexId); }

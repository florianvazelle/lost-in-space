#include "action/laser.h"
#include "level/space.h"

typedef struct {
        vector3 player;
        vector3 laser;
} double_vector3;

static double_vector3 *all_laser;
static int size = 0;

void shootf(float from[4], float to[4]) {
        vector3 player = {from[0], from[1], from[2]};
        vector3 laser = {to[0], to[1], to[2]};
        hit_shoot_satellite(player, laser);
        double_vector3 *tmp = realloc(all_laser, sizeof(double_vector3) * (size + 1));
        if (NULL == tmp) {
                free(tmp);
        } else {
                all_laser = tmp;
        }
        all_laser[size].player = player;
        all_laser[size].laser = laser;
        size++;
}

static GLuint _laserId = 0;
void draw_shoot() {

        if (_laserId == 0) {
                float color[1] = {RGB(255, 255, 0)};

                glGenTextures(1, &_laserId);
                glBindTexture(GL_TEXTURE_2D, _laserId);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                             color);
        }

        glUseProgram(_pBasicId);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(_pBasicId, "myTexture"), 0);

        glUniform1i(glGetUniformLocation(_pBasicId, "type"), 0);
        glUniform1i(glGetUniformLocation(_pBasicId, "light"), 0);
        glUniform1f(glGetUniformLocation(_pBasicId, "explosion"), 0);

        glBindTexture(GL_TEXTURE_2D, _laserId);

        for (int i = 0; i < size; i++) {
                vector3 player = all_laser[i].player;
                vector3 laser = all_laser[i].laser;

                /**
                 * Laser Translate
                 */
                float translateX = (player.x + laser.x) / 2;
                float translateY = (player.y + laser.y) / 2;
                float translateZ = (player.z + laser.z) / 2;

                /**
                 * Laser Rotation
                 */
                float deltaX = laser.x - player.x;
                float deltaY = laser.y - player.y;
                float deltaZ = laser.z - player.z;

                float len = sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

                float angleX;
                if (fabs(deltaZ) < 1.0e-3) {
                        angleX = 57.2957795 * acos(deltaX / len);
                        if (deltaY <= 0.0)
                                angleX = -angleX;
                } else {
                        angleX = 57.2957795 * acos(deltaZ / len);
                        if (deltaZ <= 0.0)
                                angleX = -angleX;
                }

                float rx = -deltaY * deltaZ;
                float ry = deltaX * deltaZ;

                gl4duBindMatrix("modelMatrix");
                gl4duLoadIdentityf();

                gl4duPushMatrix(); {
                        gl4duTranslatef(translateX, translateY, translateZ);
                        if (fabs(deltaZ) < 1.0e-3) {
                                gl4duRotatef(90.0, 0, 1, 0.0);
                                gl4duRotatef(angleX - 90, -1.0, 0.0, 0.0);
                        } else {
                                gl4duRotatef(angleX, rx, ry, 0.0);
                                gl4duRotatef(90.0, 1, 0, 0.0);
                        }
                        gl4duScalef(0.02f, 500, 0.02f);
                        gl4duSendMatrices();
                } gl4duPopMatrix();

                gl4dgDraw(_cylinder);
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        glUseProgram(0);
}

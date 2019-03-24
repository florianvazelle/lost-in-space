#include "config.h"

void init_config() {
        _pBasicId = gl4duCreateProgram("<vs>assets/shaders/basic.vs", "<gs>assets/shaders/basic.gs", "<fs>assets/shaders/basic.fs", NULL);
        _pModelId = gl4duCreateProgram("<vs>assets/shaders/model.vs", "<fs>assets/shaders/model.fs", NULL);

        _plane = gl4dgGenQuadf();
        _sphere = gl4dgGenSpheref(50, 50);

        _light = 1;
}

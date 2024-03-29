#include "config.h"

void init_config() {
        _pBasicId = gl4duCreateProgram("<vs>assets/shaders/basic.vs", "<gs>assets/shaders/basic.gs", "<fs>assets/shaders/basic.fs", NULL);
        _pModelId = gl4duCreateProgram("<vs>assets/shaders/model.vs", "<fs>assets/shaders/model.fs", NULL);
        _pStarfieldId = gl4duCreateProgram("<vs>assets/shaders/starfield.vs", "<fs>assets/shaders/starfield.fs", NULL);
        _pTextId = gl4duCreateProgram("<vs>assets/shaders/text.vs", "<fs>assets/shaders/text.fs", NULL);

        _plane = gl4dgGenQuadf();
        _sphere = gl4dgGenSpheref(50, 50);
        _cylinder = gl4dgGenCylinderf(50, 1);

        _wW = 800;
        _wH = 600;

        _xm = 400;
        _ym = 300;

        _light = 1;

        _state = MENU;
}

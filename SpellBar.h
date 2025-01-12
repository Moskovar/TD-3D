#pragma once
#include "imgui.h"

#include <iostream>
#include <string>
#include <vector>

#include "Button.h"

class SpellBar
{

public:
    SpellBar() {}
    SpellBar(short buttons_width, short buttons_height);


    void render();

    short   x = 800, y = 1000, buttons_width = 50, buttons_height = 50, offsetX = 5, offsetY = 10;

private:
    std::vector<Button> buttons;
};


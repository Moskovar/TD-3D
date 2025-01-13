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
    SpellBar(std::string name, short buttons_width, short buttons_height, int x = 800, int y = 1000);

    std::string& getName() { return name; }
    std::vector<Button>& getButtons();

    void setElement(short button, Element* element);

    void render();

    short   x = 800, y = 1000, buttons_width = 50, buttons_height = 50, offsetX = 5, offsetY = 10;

private:
    std::string name = "";
    std::vector<Button> buttons;
};


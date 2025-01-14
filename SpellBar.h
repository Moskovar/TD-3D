#pragma once
#include "UIElement.h"

#include <iostream>
#include <string>
#include <vector>

#include "Button.h"

class SpellBar : public UIElement
{

    public:
        SpellBar() {}
        SpellBar(std::string name, short buttons_width, short buttons_height, int x, int y);

        std::string& getName() { return name; }
        std::vector<Button>& getButtons();

        char* setElement(short i, Element* element);//retourne le raccourci du bouton auquel on a changé son spell
        void setShortcut(short i, char shortcut);

        void render();

        short buttons_width = 50, buttons_height = 50, offsetX = 5, offsetY = 10;

    private:
        std::string name = "";
        std::vector<Button> buttons;
};


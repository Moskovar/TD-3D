#pragma once
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "SpellBar.h"

class UI
{
	public:
		UI();
        ~UI();

		SpellBar* getBar(int i);
		std::vector<SpellBar*>& getBars() { return bars; }

		char* setSpellButton(int i, short button, Element* element);//retourne le raccouci du bouton qu'on a set
		void setShortcut(short i_bar, short i, char shortcut);

		void render();

	private:
		std::vector<SpellBar*> bars;
        //SpellBar* spellBar = nullptr, *buildBar = nullptr;
};
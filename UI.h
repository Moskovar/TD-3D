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

		void setSpellButton(int i, short button, Element* element);

		void render();

	private:
		std::vector<SpellBar*> bars;
        //SpellBar* spellBar = nullptr, *buildBar = nullptr;
};
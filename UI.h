#pragma once
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "SpellBar.h"

class UI
{
	public:
		UI();
        ~UI();

		void render();

	private:
        SpellBar* spellBar = nullptr;
};


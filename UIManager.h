#pragma once
#include "UI.h"
#include "Player.h"

class UIManager
{
	public:
		UIManager(Player* player);

		void setSpellButton(int i, short button, Spell* spell);

		void renderUI();
		void renderSpellBar();

	private:
		UI		ui;
		Player* player	= nullptr;

		void drawTextureButton(Texture* texture, short buttons_width, short buttons_height, GLfloat opacity = 1.0f);
		void drawEmptyButton(std::string name, short buttons_width, short buttons_height);
		void drawShortcut(char key);
		void drawTimer(short timeLeft);
};


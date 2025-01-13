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
};


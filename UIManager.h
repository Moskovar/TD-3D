#pragma once
#include "UI.h"
#include "Player.h"

class UIManager
{
	public:
		UIManager(Player* player, std::map<char, bool>* keyPressed);

		void setSpellButton(int i, short button, Spell* spell);
		void setShortCut(short i_bar, short i, char shortcut) { ui.setShortcut(i_bar, i, shortcut); }

		void renderUI();
		void renderSpellBar(SpellBar* bar);

		void processKeyboard();

		std::map<char*, Element*> shortcuts;//le raccourci du bouton lié avec l'élément de l'action (spell, tower ou autre)

	private:
		UI						ui;
		Player*					player		= nullptr;
		std::map<char, bool>*	keyPressed	= nullptr;//Entrées utilisateur qui sont faîtes dans le main

		void drawTextureButton(Element* element, short buttons_width, short buttons_height, GLfloat opacity = 1.0f);
		void drawEmptyButton(std::string name, short buttons_width, short buttons_height);
		void drawShortcut(char key);
		void drawTimer(short timeLeft);
		void drawProgressBar(int x, int y, GLfloat progress, ImVec4 color, std::string overlay = "");
};


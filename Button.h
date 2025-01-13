#pragma once
#include "Spell.h"
#include "Tower.h"


class Button
{
	public:
		Button() {}
		Button(char shortcut, bool showShortcut);

		Texture*	getTexture()	{ return (element) ? element->getTexture() : nullptr;	}
		Element*	getElement()	{ return element;										}

		void setElement(Element* element) { this->element = element; std::cout << "SPELL SET ->>>>> " << element << std::endl; }

		char	shortcut = 0;
		bool	showShortcut = true;

	private:
		Element* element = nullptr;
};


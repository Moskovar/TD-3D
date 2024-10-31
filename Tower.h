#pragma once
#include "Element.h"

class Tower : public Element
{
	public:
		Tower() {}
		Tower(short id, glm::vec3 position, const std::string& filePath);

		GLfloat getRange() { return range; }
		GLfloat getDamages() { return damages; }

	private:
		GLfloat range   = 0;
		GLfloat damages = 0;
};


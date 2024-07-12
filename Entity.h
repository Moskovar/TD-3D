#pragma once
#include "Element.h"
#include <string>

class Entity : public Element
{
	public:
		Entity() {}
		Entity(short id, glm::vec3 position, const std::string& filePath);

		bool isMoving() { return moving; }

		void setMove(bool state) { moving = state; }

	private:
		bool moving = false;
};


#pragma once
#include "Element.h"
#include <string>

class Entity : public Element
{
	public:
		Entity() {}
		Entity(short id, glm::vec3 position, const std::string& filePath);

		/*short getID() { return id; }*/

	private:
		//short id = -1;
};


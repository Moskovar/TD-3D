#pragma once
#include "Element.h"

class Spell : public Element
{
	public:
		Spell() {};
		Spell(short id, glm::vec3 position, const std::string& filePath);
		Spell(short id, glm::vec3 position, Model* model);
		~Spell();

		void run(const float& deltaTime);
};


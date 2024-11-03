#pragma once
#include "Entity.h"

class Character : public Entity
{
	public:
		Character() {}
		Character(short id, glm::vec3 position);
		Character(short id, glm::vec3 position, Model* model);
		~Character();

	private:
};


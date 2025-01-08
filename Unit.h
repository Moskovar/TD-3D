#pragma once
#include "Entity.h"

class Unit : public Entity
{
	public:
		Unit() {}
		Unit(short id, glm::vec3 position);
		Unit(short id, glm::vec3 position, Model* model);
		~Unit();

	private:
};


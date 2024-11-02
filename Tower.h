#pragma once
#include "Entity.h"

class Tower : public Element
{
	public:
		Tower() {}
		Tower(short id, glm::vec3 position, const std::string& filePath);

		GLfloat getRange() { return range; }
		GLfloat getDamages() { return damages; }

		virtual void attack(std::vector<Entity*>& entities, GLfloat timeSinceStart);

	private:
		GLfloat range			= 50;
		GLfloat damages			= 10;
		GLfloat attackSpeed		= 500;//milliseconds
		GLfloat lastAttackTime  = 0;//time since last attack

		bool isInRange(Element* e);
};


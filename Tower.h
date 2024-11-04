#pragma once
#include "Entity.h"

class Tower : public Element
{
	public:
		Tower() {}
		Tower(short id, glm::vec3 position, const std::string& filePath);
		Tower(short id, glm::vec3 position, Model* model);

		GLfloat getRange()		{ return range;		}
		GLfloat getDamages()	{ return damages;	}

		virtual void attack(std::vector<Entity*>& entities, const GLfloat& timeSinceStart);
		
	private:
		GLfloat range			= 50;
		GLfloat damages			= 10;
		GLfloat attackSpeed		= 0.5f;//seconds
		GLfloat lastAttackTime  = 0;//time since last attack

		bool isInRange(Element* e);
};


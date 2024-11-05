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

		virtual void attack(std::vector<Entity*>& entities);
		
	private:
		GLfloat range				= 50;
		GLfloat damages				= 10;
		unsigned int attackSpeed	= 500;//ms
		std::chrono::high_resolution_clock::time_point lastAttackTime;//time since last attack


		bool isInRange(Element* e);
};


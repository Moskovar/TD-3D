#pragma once
#include "Entity.h"

class Tower : public Element
{
	public:
		Tower() {}
		Tower(short id, glm::vec3 position, GLfloat range, GLfloat damages, const std::string& filePath);
		Tower(short id, glm::vec3 position, GLfloat range, GLfloat damages, Model* model);

		//--- Getters ---//
		GLfloat getRange()		{ return range;		}
		GLfloat getDamages()	{ return damages;	}

		//--- Setters ---//

		virtual void attack(std::vector<Entity*>& entities);
		
	protected:
		GLfloat range				= 50;
		GLfloat damages				= 10;
		unsigned int attackSpeed	= 500;//ms
		std::chrono::high_resolution_clock::time_point lastAttackTime;//time since last attack


		bool isInRange(Element* e);
};


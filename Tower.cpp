#include "Tower.h"

Tower::Tower(short id, glm::vec3 position, const std::string& filePath) : Element(id, position, filePath)
{
}

Tower::Tower(short id, glm::vec3 position, Model* model) : Element(id, position, model)
{
}

void Tower::attack(std::vector<Entity*>& entities, const GLfloat& timeSinceStart)
{
	//std::cout << "||--- attack ---||" << std::endl;
	//std::cout << timeSinceStart - lastAttackTime << " -> " << timeSinceStart << " - " << lastAttackTime << std::endl;
	if (timeSinceStart - lastAttackTime < attackSpeed) return;

	for (Entity* e : entities)
	{
		//std::cout << "Entity" << std::endl;
		if (isInRange(e))
		{
			//std::cout << "Element in range !!" << std::endl;
			e->takeDamages(this->damages);
			lastAttackTime = timeSinceStart;
			//std::cout << "ATTACK!!" << std::endl;
			break;
		}
	}
}

bool Tower::isInRange(Element* e)
{
	if (e)
		if (glm::distance(position, e->getPosition()) <= this->range) return true;

	return false;
}

#include "Tower.h"

Tower::Tower(short id, glm::vec3 position, const std::string& filePath) : Element(id, position, filePath)
{
}

void Tower::attack(std::vector<Entity*>& entities, GLfloat timeSinceStart)
{
	if (timeSinceStart - lastAttackTime < attackSpeed) return;

	for (Entity* e : entities)
	{
		if (isInRange(e))
		{
			//std::cout << "Element in range !!" << std::endl;
			e->takeDamages(this->damages);
			lastAttackTime = timeSinceStart;
			std::cout << "ATTACK!!" << std::endl;
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

#include "Tower.h"

Tower::Tower(short id, glm::vec3 position, GLfloat range, GLfloat damages, const std::string& filePath) : Element(id, position, filePath)
{
	this->range   = range;
	this->damages = damages;

	lastAttackTime = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(attackSpeed);
}

Tower::Tower(short id, glm::vec3 position, GLfloat range, GLfloat damages, Model* model) : Element(id, position, model)
{
	this->range = range;
	this->damages = damages;

	lastAttackTime = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(attackSpeed);
}

void Tower::attack(std::vector<Entity*>& entities)
{
	//std::cout << "||--- attack ---||" << std::endl;
	//std::cout << timeSinceStart - lastAttackTime << " -> " << timeSinceStart << " - " << lastAttackTime << std::endl;
	auto currentTime = std::chrono::high_resolution_clock::now();
	float timeSinceLastAttack = std::chrono::duration<float, std::milli>(currentTime - lastAttackTime).count();

	if (timeSinceLastAttack < attackSpeed) return;

	for (Entity* e : entities)
	{
		//std::cout << "Entity" << std::endl;
		if (isInRange(e))
		{
			//std::cout << "Element in range !!" << std::endl;
			e->takeDamages(this->damages);
			lastAttackTime = std::chrono::high_resolution_clock::now();
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

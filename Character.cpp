#include "Character.h"

Character::Character(short id, glm::vec3 position) : Entity(id, position, "models/fbx/doublecube.fbx")
{
}

Character::Character(short id, glm::vec3 position, Model* model) : Entity(id, position, model)
{
	//std::cout << "Character should be created at: " << position.x << " ... " << position.y << " ... " << position.z << std::endl;
	//std::cout << "Character created at: " << this->position.x << " ... " << this->position.y << " ... " << this->position.z << std::endl;
}

Character::~Character()
{
}

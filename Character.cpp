#include "Character.h"

Character::Character(short id, glm::vec3 position) : Entity(id, position, "models/fbx/doublecube.fbx")
{
	v_possessedBuilding.push_back(new Element(0, glm::vec3(0.0f, 0.0f, 0.0f), "models/obj/foundation.obj"));
}

Character::~Character()
{
	for(Element* e : v_possessedBuilding)
		if (e)
		{
			delete e;
			e = nullptr;
		}

	if (selectedBuilding)
	{
		delete selectedBuilding;
		selectedBuilding = nullptr;
	}
}

void Character::renderSelected(GLuint& modelLoc, GLuint& bonesTransformsLoc, float& timeSinceStart)
{
	
	if (selectedBuilding)
	{
		std::cout << position.x << " ... " << position.y << " ... " << position.z << std::endl;
		glm::mat4 mtx = this->modelMatrix;
		mtx = glm::translate(mtx, glm::vec3(0.0f, 0.0f, 20.0f));
		selectedBuilding->setModelMtx(mtx);
		selectedBuilding->render(modelLoc, bonesTransformsLoc, timeSinceStart);
	}
}

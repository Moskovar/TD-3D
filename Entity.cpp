#include "Entity.h"

Entity::Entity(short id, glm::vec3 position, const std::string& filePath) : Element(id, position, filePath)
{
	//std::cout << "MAXPOINT: " << model->getMaxPoint().x << std::endl;
}

Entity::Entity(short id, glm::vec3 position, Model* model) : Element(id, position, model)
{
}

void Entity::takeDamages(int damages)
{
	this->hp -= damages;
	if (hp <= 0)
	{
		std::cout << "Entity is dead RIP !" << std::endl;
		alive = false;
	}
}

void Entity::jump(GLfloat deltaTime)
{
	GLfloat value = moveSpeed * deltaTime / 2;

	if (jumpValue + value >= JUMP_HIGH) value = JUMP_HIGH - jumpValue;
	
	jumpValue += value;

	//checker le mouvement anticipé avant de le réaliser [A FAIRE]
	translate(glm::vec3(0.0f, value, 0.0f));
	updatePosition();

	//std::cout << position.y << " ... " << jumpValue << std::endl;

	if (jumpValue >= JUMP_HIGH)//Si jump atteint hauteur max, on jumping = false et la gravité reprend la main
	{
		jumping = false;
		jumpValue = 0.0f;
	}
}

void Entity::handleMovement(GLfloat deltaTime)
{
	switch (movementState)
	{
	case MovementState::IDLE:
		break;
	case MovementState::MOVING:
		break;
	}
}

void Entity::handleGravity(GLfloat deltaTime)
{
}

void Entity::updateAnimationID()
{
	switch (movingValue)
	{
		//--- Aucune touche appuyée ---//
		case DirectionValue::NONE				: animationID = 0;	break;

		//---	1 touche appuyée	---//
		case DirectionValue::FORWARD			: animationID = 1;	break;
		case DirectionValue::RIGHT				: animationID = 2;	break;
		case DirectionValue::BACKWARD			: animationID = 1;	break;
		case DirectionValue::LEFT				: animationID = 2;	break;

		//---	2 touches appuyées	---//
		case DirectionValue::FORWARD_RIGHT		: animationID = 1;	break;//1 + 2
		case DirectionValue::FORWARD_BACK		: animationID = 0;	break;//1 + 4
		case DirectionValue::FORWARD_LEFT		: animationID = 1;	break;//1 + 8
		case DirectionValue::BACK_RIGHT			: animationID = 1;	break;//2 + 4
		case DirectionValue::RIGHT_LEFT			: animationID = 0;	break;//2 + 8
		case DirectionValue::BACK_LEFT			: animationID = 1;	break;//4 + 8

		//---	3 touches appuyées	---//
		case DirectionValue::FORWARD_RIGHT_BACK	: animationID = 2;	break;//1 + 2 + 4
		case DirectionValue::FORWARD_RIGHT_LEFT	: animationID = 1;	break;//1 + 2 + 8
		case DirectionValue::FORWARD_LEFT_BACK	: animationID = 2;	break;//1 + 4 + 8
		case DirectionValue::BACK_RIGHT_LEFT	: animationID = 1;	break;//2 + 4 + 8

		//--- Toutes touches appuyées ---//
		case DirectionValue::ALL				: animationID = 0;	break;//1 + 2 + 4 + 8
	}
}

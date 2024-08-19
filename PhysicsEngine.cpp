#include "PhysicsEngine.h"

void PhysicsEngine::applyGravity(Element* element, GLfloat deltaTime)
{
	element->fall(deltaTime);
}

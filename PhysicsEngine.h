#pragma once

#include "Element.h"

class PhysicsEngine
{
	public:
		void applyGravity(Element* element, GLfloat deltaTime);
};


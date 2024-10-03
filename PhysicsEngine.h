#pragma once
#include "Map.h"
#include "Element.h"
#include "Entity.h"

class PhysicsEngine
{
	public:
		PhysicsEngine() {}
		PhysicsEngine(Game::Map* world, std::vector<Element*>* elements);
		~PhysicsEngine();

		GLfloat distanceBetweenHitboxes(Element* e1, Element* e2);
		bool checkHeightMap(Element* element, glm::vec3 nextElementPos);
		void applyGravity(Element* element, GLfloat deltaTime);
		bool checkCollision(const OBB& obb1, const OBB& obb2);

	private:
		Game::Map* world = nullptr;
		std::vector<Element*>* elements = nullptr;

		bool testAxis(const glm::vec3& axis, const OBB& obb1, const OBB& obb2, const glm::vec3& centerVector);
};


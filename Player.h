#pragma once
#include "Character.h"
#include "Nexus.h"
#include "Tower.h"

class Player
{
	public:
		Player() {}
		Player(bool rightSide);//z le plus petit = true
		~Player();

		void nexusSpawn(const GLfloat& timeSinceStart);
		void render(const GLuint& modelLoc, const GLuint& bonesTransformsLoc, const float& timeSinceStart, const float& deltaTime);

	private:
		bool rightSide						= true;

		Character*				character	= nullptr;
		Nexus*					nexus		= nullptr;

		std::vector<Entity*>	entities	= {};//seul le nexus doit modifier cette liste
		std::vector<Tower*>		towers 		= {};

		//--- Mutex ---//
		std::mutex* entities_mutex = nullptr;
};


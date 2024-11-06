#pragma once
#include "Character.h"
#include "Nexus.h"
#include "Tower.h"
#include "Cell.h"

class Player
{
	public:
		Player() {}
		Player(bool rightSide, Player* enemy);//z le plus petit = true
		~Player();

		//--- Getters ---//
		std::vector<Entity*>& getEntities() { return nexus->getEntities(); }

		//--- Setters ---//
		void setEnemy(Player* enemy) { this->enemy = enemy; }

		void addTower(int towerID, Cell& cell, const glm::vec3 worldPos);

		void nexusSpawn(const GLfloat& timeSinceStart);
		void render(const GLuint& modelLoc, const GLuint& bonesTransformsLoc, const float& timeSinceStart, const float& deltaTime);

	private:
		bool rightSide						 = true;

		Character*				character	 = nullptr;
		Nexus*					nexus		 = nullptr;

		std::vector<Tower*>		towers 		 = {};
		std::map<int, Tower>	towers_model = {};

		Player*					enemy		 = nullptr;//faire un tower manager qui gère les attaques des tourelles pour ne pas favoriser le joueur dont l'attaque des tours est appelée en première
};


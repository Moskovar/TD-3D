#include "Player.h"

Player::Player(bool rightSide, Player* enemy)
{
	this->rightSide = rightSide;
	this->enemy		= enemy;

	//--- Chargement des modèles de tours ---//
	towers_model[HumanTowers::ArcherTower] = Tower(0, glm::vec3(0.0f ,0.0f, 0.0f), "models/fbx/doublecube.fbx");

	if (rightSide)
	{
		character = new Character(0, glm::vec3(getCellCenter(996), 0.0f, getCellCenter(930)));

		nexus = new Nexus(0, glm::vec3(getCellCenter(1050), 0.0f, getCellCenter(930)), rightSide, "models/obj/r5.obj");
		nexus->turn(90.0f);
	}
	else
	{
		character = new Character(0, glm::vec3(getCellCenter(1052), 0.0f, getCellCenter(1118)));

		nexus = new Nexus(0, glm::vec3(getCellCenter(998), 0.0f, getCellCenter(1118)), rightSide, "models/obj/r5.obj");
		nexus->turn(-90.0f);
	}
}

Player::~Player()
{
	if (character)
	{
		delete character;
		character = nullptr;
	}

	for (auto pair : towers_model)
	{
		pair.second.clear();
	}

	if (nexus)
	{
		delete nexus;
		nexus = nullptr;
	}

	for (Tower* t : towers)
	{
		if (t)
		{
			delete t;
			t = nullptr;
		}
	}
}

void Player::addTower(int towerID, Cell& cell, const glm::vec3 worldPos)
{
	Tower* tower = new Tower(0, glm::vec3(((int)worldPos.x / 8) * 8 + 4, 0.0f, ((int)worldPos.z / 8) * 8 + 4), towers_model[towerID].getModel());
	towers.push_back(tower);
	cell.setTower(tower);
}

void Player::nexusSpawn(const GLfloat& timeSinceStart)
{
	nexus->play(rightSide, timeSinceStart);
}

void Player::render(const GLuint& modelLoc, const GLuint& bonesTransformsLoc, const float& timeSinceStart, const float& deltaTime)
{
	if (!character) { std::cout << "Character nullptr!" << std::endl; return; }
	if (!nexus)		{ std::cout << "nexus nullptr!"		<< std::endl; return; }

	character->render(modelLoc, bonesTransformsLoc);
	nexus->render(modelLoc, bonesTransformsLoc, deltaTime);

	for (Tower* t : towers)
	{
		t->attack(enemy->getEntities());
		t->render(modelLoc, bonesTransformsLoc);
	}

	//std::cout << entities.size() << std::endl;
}

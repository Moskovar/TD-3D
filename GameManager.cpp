#include "GameManager.h"

GameManager::GameManager(Player* rPlayer, Player* lPlayer, float* deltaTime)
{
	this->rPlayer   = rPlayer;
	this->lPlayer   = lPlayer;
	this->deltaTime = deltaTime;
}

GameManager::~GameManager()
{
}

void GameManager::manage()
{
	manageUnits();
}

void GameManager::manageTower()
{
}

void GameManager::manageUnits()
{
	//--- Déplacements ---//
	for (Entity* e : rPlayer->getEntities())
	{
		if (!e || !e->isAlive()) continue;

		if (e->getPosition().z < getCellCenter(1091)) e->move(*deltaTime);
		else e->setAnimationID(2);
	}

	for (Entity* e : lPlayer->getEntities())
	{
		if (!e || !e->isAlive()) continue;

		if (e->getPosition().z > getCellCenter(957)) e->move(-*deltaTime);
		else e->setAnimationID(2);
	}
}

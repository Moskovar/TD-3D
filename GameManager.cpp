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
	manageTowers();
}

void GameManager::manageTowers()//doublons parcour des tours (autre parcours pour affichage)
{
	for (Tower* t : rPlayer->getTowers())
	{
		t->attack(lPlayer->getEntities());
	}

	for (Tower* t : lPlayer->getTowers())
	{
		t->attack(rPlayer->getEntities());
	}
}

void GameManager::manageUnits()
{
	//--- Déplacements ---//
	for (Entity* e : rPlayer->getEntities())//doublon parcours (autre pour affichage)
	{
		if (!e || !e->isAlive()) continue;

		if (e->getPosition().z < getCellCenter(1091)) e->move(*deltaTime);
		else e->setAnimationID(2);
	}

	for (Entity* e : lPlayer->getEntities())//doublons parcours (autre pour affichage)
	{
		if (!e || !e->isAlive()) continue;

		if (e->getPosition().z > getCellCenter(957)) e->move(-*deltaTime);
		else e->setAnimationID(2);
	}
}

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
	manageSpells();
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

void GameManager::manageSpells()
{
	for (Spell* spell : rPlayer->getSpells())
	{
		if (!spell) continue;
		spell->run(-*deltaTime);
	}

	for (Spell* spell : lPlayer->getSpells())
	{
		if (!spell) continue;
		spell->run(*deltaTime);
		for (Entity* e : rPlayer->getEntities())
		{
			if (isPointInsideOBB(spell->getRHitbox().center, e->getRHitbox()))
			{
				std::cout << "HIT!" << std::endl;
				e->takeDamages(50);
				spell->terminate();
			}
		}
		//std::cout << spell->getRHitbox().center.z << std::endl;
	}

	//std::cout << "Nombre de spells: " << lPlayer->getSpells().size() << std::endl;
	//Gestion des spells de lPlayer
	for (auto it = lPlayer->getSpells().begin(); it != lPlayer->getSpells().end();) 
	{
		if(*it != nullptr && !(*it)->isOver())
		{
			(*it)->run(*deltaTime);
			for (Entity* e : rPlayer->getEntities())
			{
				if (isPointInsideOBB((*it)->getRHitbox().center, e->getRHitbox()))
				{
					//std::cout << "HIT!" << std::endl;
					e->takeDamages(50);
					(*it)->terminate();
					break;
				}
			}
		}

		if (*it == nullptr || (*it)->isOver()) { it = lPlayer->getSpells().erase(it); std::cout << "Suppression spell" << std::endl; }// Supprimer les nullptr et passe à l'élément suivant
		else									++it; // Passer au suivant
	}

	//std::cout << "Nombre de spells: " << lPlayer->getSpells().size() << std::endl;

}

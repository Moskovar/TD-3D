#include "Player.h"

Player::Player(bool rightSide)
{
	this->rightSide = rightSide;

	if (rightSide)
	{
		character = new Character(0, glm::vec3(getCellCenter(996), 0.0f, getCellCenter(930)));

		nexus = new Nexus(0, glm::vec3(getCellCenter(1050), 0.0f, getCellCenter(930)), &entities, "models/obj/r5.obj");
		nexus->turn(90.0f);

		entities_mutex = &r_entities_mutex;
	}
	else
	{
		character = new Character(0, glm::vec3(getCellCenter(1052), 0.0f, getCellCenter(1118)));

		nexus = new Nexus(0, glm::vec3(getCellCenter(998), 0.0f, getCellCenter(1118)), &entities, "models/obj/r5.obj");
		nexus->turn(-90.0f);

		entities_mutex = &l_entities_mutex;
	}
}

Player::~Player()
{
	if (character)
	{
		delete character;
		character = nullptr;
	}

	if (nexus)
	{
		delete nexus;
		nexus = nullptr;
	}

	for (Entity* e : entities)
	{
		if (e)
		{
			delete e;
			e = nullptr;
		}
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

void Player::nexusSpawn(const GLfloat& timeSinceStart)
{
	nexus->spawn(rightSide, timeSinceStart);
}

void Player::render(const GLuint& modelLoc, const GLuint& bonesTransformsLoc, const float& timeSinceStart, const float& deltaTime)
{
	if (!character) { std::cout << "Character nullptr!" << std::endl; return; }
	if (!nexus)		{ std::cout << "nexus nullptr!"		<< std::endl; return; }

	character->render(modelLoc, bonesTransformsLoc, timeSinceStart);
	nexus->render(modelLoc, bonesTransformsLoc, timeSinceStart);

	//std::cout << entities.size() << std::endl;

	entities_mutex->lock();
	for (Entity* e : entities)
	{
		if (!e || !e->isAlive()) continue;

		if (rightSide)
		{
			if (e->getPosition().z < getCellCenter(1091)) e->move(deltaTime); //pas vraiment sa place ici mais ça évite de refaire un parcours d'entities
		}
		else			
		{
			if (e->getPosition().z > getCellCenter(957)) e->move(-deltaTime); //pas vraiment sa place ici mais ça évite de refaire un parcours d'entities
		}
		e->render(modelLoc, bonesTransformsLoc, timeSinceStart);
	}
	entities_mutex->unlock();
}

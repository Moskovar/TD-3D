#include "Nexus.h"
#include <thread>
#include <chrono>

Nexus::Nexus(short id, glm::vec3 position, const bool& rightSide, const std::string& filePath) : Element(id, position, filePath)
{
	this->rightSide = rightSide;

	if (rightSide)
	{
		int x = getCellCenter(1052);
		entities_model.push_back(Character(0, glm::vec3(x, 0.0f, 0)));
	}
	else
	{
		int x = getCellCenter(996);
		entities_model.push_back(Character(0, glm::vec3(x, 0.0f, 0)));
	}
}

Nexus::~Nexus()
{
	for (Entity* e : entities)
	{
		if (e)
		{
			delete e;
			e = nullptr;
		}
	}

	for (Character c : entities_model)
		c.clear();
}

void Nexus::play(const bool& rightSide, const GLfloat& timeSinceStart)
{
	//std::cout << timeSinceStart - lastWaveTime << " : " << spawnFrequency << std::endl;
	if (entities_loaded.size() < nbPerWave)
	{
		//std::cout << "EXAMPLE MODEL ADDR: " << entities_model[0].getModel() << std::endl;
		if (rightSide)
		{
			int x = getCellCenter(1052), z = getCellCenter(930);
			entities_loaded.push_back(new Character(0, glm::vec3(x, 0.0f, z - entities_loaded.size() * 5), entities_model[0].getModel()));

		}
		else
		{
			int x = getCellCenter(996), z = getCellCenter(1118);
			entities_loaded.push_back(new Character(0, glm::vec3(x, 0.0f, z + (nbPerWave - 1 - entities_loaded.size()) * 5), entities_model[0].getModel()));
		}
	}

	cleanEntities();

	if (timeSinceStart - lastWaveTime < spawnFrequency) return;

	for (Entity* e : entities_loaded)
	{
		entities.push_back(e);
	}
	
	entities_loaded.clear();

	lastWaveTime = timeSinceStart;
}

void Nexus::cleanEntities()
{
	entities.erase(
		std::remove_if(entities.begin(), entities.end(), [](Entity* entity)
			{
				if (entity && !entity->isAlive())
				{
					delete entity;
					entity = nullptr;
				}
				return entity == nullptr;
			}),
		entities.end()
	);
}

void Nexus::render(const GLuint& modelLoc, const GLuint& bonesTransformsLoc, const GLfloat& deltaTime)
{
	Element::render(modelLoc, bonesTransformsLoc);

	for (Entity* e : entities)
	{
		if (!moveEntity(e, deltaTime)) continue; //pas vraiment sa place ici mais ça évite de refaire un parcours d'entities dans moveEntity
		e->render(modelLoc, bonesTransformsLoc);
	}
}

bool Nexus::moveEntity(Entity* e, const GLfloat& deltaTime)
{
	if (!e || !e->isAlive()) return false;

	if (rightSide)
	{
		if (e->getPosition().z < getCellCenter(1091)) e->move(deltaTime);
		else e->setAnimationID(2);
	}
	else
	{
		if (e->getPosition().z > getCellCenter(957)) e->move(-deltaTime);
		else e->setAnimationID(2);
	}

	return true;
}

#pragma once
#include "Character.h"
#include <thread>

class Nexus : public Element
{

	public:
		Nexus(short id, glm::vec3 position, std::vector<Entity*>* entities, const std::string& filePath);

		void spawn(const bool& rightSide, const GLfloat& timeSinceStart);
		void cleanEntities();

		void start(bool rightSide, const GLboolean& run);
	
	private:
		GLfloat spawnFrequency				  = 15.0f  , lastWaveTime = 0.0f;
		unsigned short nbSpawned			  = 0      , nbPerWave = 5;
		std::vector<Entity*>* entities		  = nullptr;
		std::vector<Entity*>  entities_loaded = {};
};


#pragma once
#include "Character.h"
#include <thread>

class Nexus : public Element
{

	public:
		Nexus(short id, glm::vec3 position, const bool& rightSide, const std::string& filePath);
		~Nexus();

		//--- Getters ---//
		std::vector<Entity*>& getEntities() { return this->entities; }

		void play(const bool& rightSide, const GLfloat& timeSinceStart);
		void cleanEntities();
		
		void render(const GLuint& modelLoc, const GLuint& bonesTransformsLoc, const float& timeSinceStart, const GLfloat& deltaTime);

	private:
		bool rightSide = false;
		GLfloat			spawnFrequency	= 5.0f  , lastWaveTime = 0.0f;
		unsigned short	nbSpawned		= 0      , nbPerWave = 5;

		std::vector<Entity*>	entities = {};
		std::vector<Entity*>	entities_loaded = {};
		std::vector<Character>	entities_model = {};

		bool moveEntity(Entity* e, const GLfloat& deltaTime);//return false si l'entité est morte ou nullptr
};


#pragma once
#include "Entity.h"
#include "Spell.h"

class Character : public Entity
{
	public:
		Character() {}
		Character(short id, glm::vec3 position);
		Character(short id, glm::vec3 position, Model* model);
		~Character();

		short getResources()	{ return resources;		}
		short getMaxResources()	{ return maxResources;	}

		void addSpell(int spellID);
		void render(const GLuint& modelLoc, const GLuint& bonesTransformsLoc, const float& deltaTime);

	private:
		std::map<int, Spell>	spells_model = {};
		std::vector<Spell*>		spells;

		short resources = 0, maxResources = 100;

		void createSpellsModel();
};


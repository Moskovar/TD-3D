#pragma once
#include "Entity.h"
#include "Spell.h"
#include <thread>
#include <fmod.hpp>
#include <fmod_errors.h>

class Character : public Entity
{
	public:
		Character() {}
		Character(short id, glm::vec3 position);
		Character(short id, glm::vec3 position, Model* model);
		~Character();

		//--- Getters ---//
		short					getResources()		{ return resources;		}
		short					getMaxResources()	{ return maxResources;	}
		std::vector<Spell*>&	getSpells()			{ return spells;		}


		void addRessources(short amount)	{ resources		+= amount;	}
		void setTalk(bool state)			{ this->talking	= state;	}

		short addSpell(int spellID);
		void render(const GLuint& modelLoc, const GLuint& bonesTransformsLoc, const float& deltaTime);

	private:
		std::map<int, Spell>	spells_model = {};
		std::vector<Spell*>		spells;

		short resources = 100, maxResources = 100;

		std::unique_ptr<std::thread> t_talk;
		bool talking = false;

		void createSpellsModel();
};


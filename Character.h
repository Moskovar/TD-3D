#pragma once
#include "Entity.h"
#include "Spell.h"
#include <thread>
#include <fmod.hpp>
#include <fmod_errors.h>

#include "Voice.h"

class Character : public Entity
{
	public:
		Character() {}
		Character(short id, glm::vec3 position, FMOD::System* system);
		Character(short id, glm::vec3 position, Model* model, FMOD::System* system);
		~Character();

		//--- Getters ---//
		short					getResources()		{ return resources;		}
		short					getMaxResources()	{ return maxResources;	}
		std::vector<Spell*>&	getSpells()			{ return spells;		}


		void addRessources(short amount)	{ resources		+= amount;	}
		void setTalk(bool state)			{ this->talking	= state;	}
		void useVoice(int soundsID);

		short addSpell(int spellID);
		void render(const GLuint& modelLoc, const GLuint& bonesTransformsLoc, const float& deltaTime);

	private:
		std::map<int, Spell>	spells_model = {};
		std::vector<Spell*>		spells;

		short resources = 100, maxResources = 100;

		std::unique_ptr<std::thread> t_talk;
		bool talking = false;

		Voice* voice = nullptr;
		FMOD::System* system = nullptr;

		void createSpellsModel();
		bool loadVoices(FMOD::System* system);
};


#pragma once
#include "Element.h"

class Spell : public Element
{
	public:
		Spell() {};
		Spell(short id, glm::vec3 position, const std::string& filePath);
		Spell(short id, glm::vec3 position, Model* model);
		~Spell();

		bool isOver()		{ return over;	}
		short getCost()		{ return cost;	}

		void terminate()	{ over = true;	}

		void run(const float& deltaTime);

	private:
		bool over = false;//si le spell est terminé et est prêt à être supprimé (comme alive pour une entity)
		short cost = 75;
};


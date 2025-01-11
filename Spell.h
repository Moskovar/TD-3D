#pragma once
#include "Element.h"

class Spell : public Element
{
	public:
		Spell() {};
		Spell(short id, glm::vec3 position, const std::string& filePath);
		Spell(short id, glm::vec3 position, Model* model);
		~Spell();

		bool  isOver()		{ return over;							}
		short getCost()		{ return cost;							}
		bool  isAvailable();

		void terminate()	{ over = true;							}
		void use(int now)	{ lastTimeUsed = now;					}

		void run(const float& deltaTime);

	private:
		bool    over	= false;//si le spell est termin� et est pr�t � �tre supprim� (comme alive pour une entity)
		short   cost	= 75;
		int		cd		= 3500, lastTimeUsed = 0;
};


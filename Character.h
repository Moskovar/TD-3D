#pragma once
#include "Entity.h"

class Character : public Entity
{
	public:
		Character(short id, glm::vec3 position);
		~Character();

		//--- Getters ---//
		Element* getSelectedBuilding()	{ return selectedBuilding;						}

		void setSelected()				{ selectedBuilding = v_possessedBuilding[0];	}
		void clearSelected()			{ selectedBuilding = nullptr;					}

		void renderSelected(GLuint& modelLoc, GLuint& bonesTransformsLoc, float& timeSinceStart);

	private:
		Element* selectedBuilding = nullptr;
		std::vector<Element*> v_possessedBuilding;
};


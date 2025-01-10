#include "Character.h"

Character::Character(short id, glm::vec3 position) : Entity(id, position, "models/fbx/doublecube.fbx")
{
	createSpellsModel();
}

Character::Character(short id, glm::vec3 position, Model* model) : Entity(id, position, model)
{
	createSpellsModel();
	//std::cout << "Character should be created at: " << position.x << " ... " << position.y << " ... " << position.z << std::endl;
	//std::cout << "Character created at: " << this->position.x << " ... " << this->position.y << " ... " << this->position.z << std::endl;
}

Character::~Character()
{
	for(Spell* spell : spells)
	{
		if (spell)
		{
			delete spell;
			spell = nullptr;
		}
	}
}

short Character::addSpell(int spellID)
{
	if (resources < spellsCost[spellID]) return SpellsError::OOM;//

	Spell* spell = new Spell(0, position + glm::vec3(0.0f, 2.0f, -2.0f), spells_model[spellID].getModel());

	//on donne les caractéristiques du spell
	switch (spellID)
	{
		case Spells::FireBall: spell->setMoveSpeed(50); break;
	}

	spells.push_back(spell);

	return SpellsError::Success;//success
}

void Character::render(const GLuint& modelLoc, const GLuint& bonesTransformsLoc, const float& deltaTime)
{
	Element::render(modelLoc, bonesTransformsLoc);

	for (Spell* spell : spells)
	{
		if (spell && !spell->isOver()) spell->render(modelLoc, bonesTransformsLoc);
	}
}

void Character::createSpellsModel()
{
	spells_model[Spells::FireBall] = Spell(0, glm::vec3(0.0f, 0.0f, 0.0f), "models/fbx/fireball.fbx");
}

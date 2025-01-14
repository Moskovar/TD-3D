#include "Spell.h"

Spell::Spell(short id, short spellID, glm::vec3 position, const std::string& filePath) : Element(id, position, filePath)
{
	this->elementID = spellID;

	texture = new Texture(spells_texturesPath[spellID]);

	std::cout << "--------------- " << texture << std::endl;

	switch (spellID)
	{
		case ElementsID::FireBall: 
			moveSpeed = 50; 
			cost = spellsCost[spellID]; break;
	}
}

Spell::Spell(short id, short spellID, glm::vec3 position, Model* model) : Element(id, position, model)
{
	this->elementID = spellID;

	texture = new Texture(spells_texturesPath[spellID]);

	switch (spellID)
	{
		case ElementsID::FireBall: moveSpeed = 50; break;
	}
}

Spell::~Spell()
{

}

bool Spell::isCd()
{
	return !((getNow() - lastTimeUsed) >= 3500);
}

int Spell::getCdLeft()
{
	int cdLeft = cd - (getNow() - lastTimeUsed);//temps restant avant disponible

	//std::cout << cd << " : " << getNow() << " : " << lastTimeUsed << std::endl;

	//std::cout << "CDLEFT: " << cdLeft << std::endl;

	return (cdLeft < cd && cdLeft >= 0) ? cdLeft : 0;
}

void Spell::run(const float& deltaTime)
{
	if (over) return;
	move(-deltaTime);
}

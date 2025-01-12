#include "Spell.h"

Spell::Spell(short id, short spellID, glm::vec3 position, const std::string& filePath) : Element(id, position, filePath)
{
	texture = new Texture("textures/spells/fireball.jpg");
}

Spell::Spell(short id, short spellID, glm::vec3 position, Model* model) : Element(id, position, model)
{
	texture = new Texture("textures/spells/fireball.jpg");
}

Spell::~Spell()
{
	if (texture)
	{
		delete texture;
		texture = nullptr;
	}
}

bool Spell::isAvailable()
{
	return (getNow() - lastTimeUsed) >= 3500;
}

void Spell::run(const float& deltaTime)
{
	if (over) return;
	move(-deltaTime);
}

#include "Spell.h"

Spell::Spell(short id, glm::vec3 position, const std::string& filePath) : Element(id, position, filePath)
{
}

Spell::Spell(short id, glm::vec3 position, Model* model) : Element(id, position, model)
{
}

Spell::~Spell()
{
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

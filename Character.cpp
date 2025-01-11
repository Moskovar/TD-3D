#include "Character.h"

Character::Character(short id, glm::vec3 position, FMOD::System* system) : Entity(id, position, "models/fbx/doublecube.fbx")
{
    loadVoices(system);
    createSpellsModel();
}

Character::Character(short id, glm::vec3 position, Model* model, FMOD::System* system) : Entity(id, position, model)
{
	loadVoices(system);
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

void Character::useVoice(int soundID)
{
    voice->use(soundID);
}

void talk(Character* character, int soundID)
{
    character->setTalk(true);

    character->useVoice(soundID);

    character->setTalk(false);
}

short Character::addSpell(int spellID)
{
	//check cd
	if (!spells_model[spellID].isAvailable())
	{
		if (t_talk && !talking) t_talk->join();
		if (!talking)           t_talk = std::make_unique<std::thread>(talk, this, SoundsID::CD);
		return SpellsError::E_CD;//
	}

	//check mana
    if (resources < spellsCost[spellID])//si pas assez de mana, on joue la voice OOM puis on retourne l'erreur OOM
    {
        if (t_talk && !talking) t_talk->join();
        if (!talking)           t_talk = std::make_unique<std::thread>(talk, this, SoundsID::OOM);
        return SpellsError::E_OOM;//
    }

	Spell* spell = new Spell(0, position + glm::vec3(0.0f, 2.0f, -2.0f), spells_model[spellID].getModel());

	//on donne les caractéristiques du spell
	switch (spellID)
	{
		case Spells::FireBall: spell->setMoveSpeed(50); break;
	}

	spells.push_back(spell);
	spells_model[spellID].use(getNow());
	//std::cout << "TIMESTAMP: " << getNow() << std::endl;


	resources -= spellsCost[spellID];

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

bool Character::loadVoices(FMOD::System* system)
{
    if (!system) return false;

    voice = new Voice(Heros::TitusMage, system);
}

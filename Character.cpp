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

    //if(t_talk)
    //{
    //    if (t_talk->joinable()) t_talk->join();
    //    delete t_talk;
    //    t_talk = nullptr;
    //}

    
}

void talk(Character* character)
{
    FMOD::System* system = nullptr;
    FMOD::Sound* sound = nullptr;
    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result;

    // Initialisation du système FMOD
    result = FMOD::System_Create(&system);
    if (result != FMOD_OK) {
        std::cerr << "FMOD System Create Failed: " << FMOD_ErrorString(result) << std::endl;
        return;
    }

    result = system->init(512, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        std::cerr << "FMOD Init Failed: " << FMOD_ErrorString(result) << std::endl;
        return;
    }

    // Charger le fichier WAV sans boucle (FMOD_LOOP_OFF)
    result = system->createSound("sounds/voix1.wav", FMOD_DEFAULT, nullptr, &sound);
    if (result != FMOD_OK) {
        std::cerr << "FMOD Create Sound Failed: " << FMOD_ErrorString(result) << std::endl;
        return;
    }

    // Empêcher la boucle du son
    result = sound->setMode(FMOD_LOOP_OFF);
    if (result != FMOD_OK) {
        std::cerr << "FMOD Set Mode Failed: " << FMOD_ErrorString(result) << std::endl;
        return;
    }

    // Jouer le son
    result = system->playSound(sound, nullptr, false, &channel);
    if (result != FMOD_OK) {
        std::cerr << "FMOD Play Sound Failed: " << FMOD_ErrorString(result) << std::endl;
        return;
    }

    character->setTalk(true);

    // Attendre que le son se termine
    bool isPlaying = true;
    while (isPlaying)
    {
        system->update();
        channel->isPlaying(&isPlaying);
    }

    // Libérer les ressources !!! ON LIBERE PAS SI ERREUR AVANT !!!
    sound->release();
    system->close();
    system->release();

    character->setTalk(false);
}

short Character::addSpell(int spellID)
{
    if (resources < spellsCost[spellID])
    {
        if (t_talk && !talking) t_talk->join();
        if (!talking)            t_talk = std::make_unique<std::thread>(talk, this);
        return SpellsError::OOM;//
    }

	Spell* spell = new Spell(0, position + glm::vec3(0.0f, 2.0f, -2.0f), spells_model[spellID].getModel());

	//on donne les caractéristiques du spell
	switch (spellID)
	{
		case Spells::FireBall: spell->setMoveSpeed(50); break;
	}

	spells.push_back(spell);

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

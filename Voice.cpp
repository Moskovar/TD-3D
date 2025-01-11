#include "Voice.h"
#include <random>

#define _OOM_ 3
#define _CD_ 3

Voice::Voice(short herosID, FMOD::System* system)
{
    this->system = system;
	FMOD_RESULT result;
    //OOM
    for (int i = 0; i < _OOM_; ++i)
    {
        FMOD::Sound* sound = nullptr;
        // Charger le fichier WAV sans boucle (FMOD_LOOP_OFF)
        std::string path = "sounds/" + str_heros[herosID] + "/" + str_sounds[SoundsID::OOM] + "/" + std::to_string(i + 1) + ".wav";
        std::cout << path << std::endl;
        result = system->createSound(path.data(), FMOD_DEFAULT, nullptr, &sound);
        if (result != FMOD_OK) 
        {
            std::cerr << "FMOD Create Sound Failed: " << FMOD_ErrorString(result) << std::endl;
            continue;
        }

        // Empêcher la boucle du son
        result = sound->setMode(FMOD_LOOP_OFF);
        if (result != FMOD_OK) 
        {
            std::cerr << "FMOD Set Mode Failed: " << FMOD_ErrorString(result) << std::endl;
            continue;
        }

        voices[SoundsID::OOM].push_back(sound);
    }

    //CD
    for (int i = 0; i < _CD_; ++i)
    {
        FMOD::Sound* sound = nullptr;
        // Charger le fichier WAV sans boucle (FMOD_LOOP_OFF)
        std::string path = "sounds/" + str_heros[herosID] + "/" + str_sounds[SoundsID::CD] + "/" + std::to_string(i + 1) + ".wav";
        std::cout << path << std::endl;
        result = system->createSound(path.data(), FMOD_DEFAULT, nullptr, &sound);
        if (result != FMOD_OK)
        {
            std::cerr << "FMOD Create Sound Failed: " << FMOD_ErrorString(result) << std::endl;
            continue;
        }

        // Empêcher la boucle du son
        result = sound->setMode(FMOD_LOOP_OFF);
        if (result != FMOD_OK)
        {
            std::cerr << "FMOD Set Mode Failed: " << FMOD_ErrorString(result) << std::endl;
            continue;
        }

        voices[SoundsID::CD].push_back(sound);
    }
}

void Voice::use(int soundID)
{
    if (voices[soundID].size() == 0) return;

    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result;

    // Générateur de nombres aléatoires
    std::random_device rd; // Génère une graine aléatoire
    std::mt19937 gen(rd()); // Mersenne Twister generator
    std::uniform_int_distribution<int> distrib(0, voices[soundID].size() - 1); // Distribution entre 0 et 2 inclus

    // Obtenir un chiffre aléatoire entre 0 et 3 (exclu)
    int random_number = distrib(gen);

    std::cout << voices[soundID].size() << " : " << random_number << std::endl;

    // Jouer le son
    result = system->playSound(voices[soundID][random_number], nullptr, false, &channel);
    if (result != FMOD_OK) 
    {
        std::cerr << "FMOD Play Sound Failed: " << FMOD_ErrorString(result) << std::endl;
        return;
    }

    // Attendre que le son se termine
    bool isPlaying = true;
    while (isPlaying)
    {
        system->update();
        channel->isPlaying(&isPlaying);
    }
}
/*FMOD::System* system = nullptr;
	FMOD::Sound* sound = nullptr;
	FMOD::Channel* channel = nullptr;*/
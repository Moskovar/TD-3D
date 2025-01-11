#pragma once
#include <fmod.hpp>
#include <fmod_errors.h>
#include <map>
#include <vector>
#include "uti.hpp"

class Voice
{
	public:
		Voice() {}
		Voice(short herosID, FMOD::System* system);

		void use(int soundID);

	private:
		FMOD::System* system = nullptr;
		std::map<int, std::vector<FMOD::Sound*>> voices;
};


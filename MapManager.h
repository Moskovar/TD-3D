#pragma once
#include "Map.h"

class MapManager
{
	public:
		MapManager() {}

		void loadMap(Game::Map* map, std::map<std::string, Shader>& shaders);

	private:
};


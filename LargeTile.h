#pragma once
#include <vector>

#include "Tile.h"

class LargeTile
{
	public:
		LargeTile(int x, int y, int tilesSize);

	private:
		int x = 0, y = 0, tilesSize = 0;
		std::vector<Tile> v_tiles	= {};
};


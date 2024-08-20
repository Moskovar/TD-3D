#pragma once
#include "uti.hpp"

#define TILE_SIZE 32

class Tile
{
	public:
		Tile() {};
		Tile(int x, int y, HeightMapVertex* vertices);
		~Tile();

	private:
		int x = 0, y = 0;

		HeightMapVertex* vertices = nullptr;;
};


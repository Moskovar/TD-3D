#pragma once
#include <iostream>
#include <vector>
#include <cmath>

#include "Tile.h"

#include "stb_image.h"

#define LARGETILE_SIZE		512
#define LARGETILE_ARR_SIZE	16

class LargeTile
{
	public:
		LargeTile() {};
		LargeTile(int x, int y, const char* heightmapPath);

		Tile getTile(int y, int x) { return tiles[y][x]; }

	private:
		int x = 0, y = 0;
		Tile tiles[LARGETILE_ARR_SIZE][LARGETILE_ARR_SIZE];
};


#pragma once
#include <GL/GL.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "Tile.h"

#include "stb_image.h"

#define LARGETILE_SIZE 16

class LargeTile
{
	public:
		LargeTile(const char* heightmapPath, int x, int y);

	private:
		int x = 0, y = 0;
		Tile tiles[LARGETILE_SIZE][LARGETILE_SIZE];
};


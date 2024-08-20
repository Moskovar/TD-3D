#include "Tile.h"

Tile::Tile(int x, int y, HeightMapVertex* vertices)
{
	this->x = x;
	this->y = y;

	this->vertices = vertices;
}

Tile::~Tile()
{
	if (vertices)
	{
		delete vertices;
		vertices = nullptr;
	}

	printf("||--- Tile cleared ---||\n");
}

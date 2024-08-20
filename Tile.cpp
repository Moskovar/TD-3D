#include "Tile.h"

Tile::Tile(HeightMapVertex* vertices, int x, int y)
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

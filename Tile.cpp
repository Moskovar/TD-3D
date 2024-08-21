#include "Tile.h"
#include <iostream>

Tile::Tile()
{
	this->vertices = new HeightMapVertex * [TILE_SIZE];

	for (int y = 0; y < TILE_SIZE; ++y)
		vertices[y] = new HeightMapVertex[TILE_SIZE];
}

void Tile::setVectors()
{
    for (int y = 0; y < TILE_SIZE; ++y)
        for (int x = 0; x < TILE_SIZE; ++x)
        {//   std::cout << y << " ... " << x << std::endl;
            v_vertices.push_back(vertices[y][x]);
            if (x < TILE_SIZE - 1 && y < TILE_SIZE - 1)
            {
                //triangle de gauche
                v_indices.push_back(vertices[y][x].indice);
                v_indices.push_back(vertices[y][x + 1].indice);
                v_indices.push_back(vertices[y + 1][x].indice);
            }
    
            if (x > 0 && y < TILE_SIZE - 1)
            {
                ////triangle de droite
                v_indices.push_back(vertices[y][x].indice);
                v_indices.push_back(vertices[y + 1][x].indice);
                v_indices.push_back(vertices[y + 1][x - 1].indice);
            }
        }
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

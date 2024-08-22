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
		LargeTile(int x, int y, const char* heightmapPath);

		//--- Getters ---//
		Tile getTile(int y, int x) { return tiles[y][x]; }

		void setBuffers();

		void render();

	private:
		int x = 0, y = 0;

		GLuint VAO, VBO, IBO;

		Tile tiles[LARGETILE_ARR_SIZE][LARGETILE_ARR_SIZE];
		HeightMapVertex junction_vertices[(TILE_SIZE - 2) * 2][TILE_SIZE];//Il y a TILE_SIZE - 2 joint à faire sur l'axe z (y du tableau)
																		  //* 2 car chaque jointure prendre les vertices haut d'une tuile et bas de la suivante (donc pas réutilisable d'une jointure à l'autre)
		std::vector<HeightMapVertex> v_vertices = {};
		std::vector<unsigned int>	 v_indices	= {};
};


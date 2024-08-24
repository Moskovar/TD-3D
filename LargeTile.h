#pragma once
#include <iostream>
#include <vector>
#include <cmath>

#include "Tile.h"

#include "stb_image.h"
#include <tinyexr.h>


#define LARGETILE_SIZE			512
#define LARGETILE_ARR_SIZE		16
#define JUNCTION_VERTICES_SIZE	2 * (LARGETILE_ARR_SIZE - 1) //* 2 car chaque jointure prendre les vertices haut d'une tuile et bas de la suivante (donc pas réutilisable d'une jointure à l'autre)
															//Il y a TILE_SIZE - 1 joint à faire sur l'axe z (y du tableau) | y: nombre de Tile, x: nombre de vertex sur une rangée X d'une Tile																	
class LargeTile
{
	public:
		LargeTile(int x, int y, const char* heightmapPath);

		//--- Getters ---//
		Tile getTile(int y, int x) { return tiles[y][x]; }
		HeightMapVertex getVertex(int y, int x) { return tiles[y / TILE_SIZE][x / TILE_SIZE].getVertex(y % TILE_SIZE, x % TILE_SIZE); }

		void setBuffers();

		void render();

	private:
		int x = 0, y = 0;

		GLuint VAO, VBO, IBO;

		Tile tiles[LARGETILE_ARR_SIZE][LARGETILE_ARR_SIZE];
		HeightMapVertex junction_vertices[2][LARGETILE_ARR_SIZE][JUNCTION_VERTICES_SIZE][TILE_SIZE];//2 -> 0 = les jointures y et 1 = les jointures x
																		  
		std::vector<HeightMapVertex> v_vertices = {};
		std::vector<unsigned int>	 v_indices	= {};
};


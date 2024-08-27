#pragma once
#include <iostream>
#include <vector>
#include <cmath>

#include "Tile.h"
#include "Texture.h"

#include <tinyexr.h>
																
class LargeTile
{
	public:
		LargeTile(int y, int x, int yChunk, int xChunk, std::string heightMapName, std::string textureName);

		//--- Getters ---//
		Tile getTile(int y, int x) { return tiles[y][x]; }
		HeightMapVertex getVertex(int y, int x) { return tiles[y / TILE_SIZE][x / TILE_SIZE].getVertex(y % TILE_SIZE, x % TILE_SIZE); }

		void render();

		~LargeTile();

	private:
		int x = 0, y = 0;

		Texture* texture = nullptr;

		GLuint VAO, VBO, IBO;

		Tile tiles[LARGETILE_ARR_SIZE][LARGETILE_ARR_SIZE];
		HeightMapVertex junction_vertices[2][LARGETILE_ARR_SIZE][JUNCTION_VERTICES_SIZE][TILE_SIZE];//2 -> 0 = les jointures y et 1 = les jointures x
																		  
		std::vector<HeightMapVertex> v_vertices = {};
		std::vector<unsigned int>	 v_indices	= {};

		void setJunctions();
};


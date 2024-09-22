#pragma once
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <cmath>

#include "Shader.h"
#include "Tile.h"
#include "Texture.h"

#include <tinyexr.h>
																
class LargeTile
{
	public:
		LargeTile(int y, int x, int yChunk, int xChunk, std::string heightMapName, std::string textureName, std::map<std::string, Shader>& shaders);

		//--- Getters ---//
		int				getX()					{ return x;																				}
		int				getY()					{ return y;																				}
		Tile			getTile(int y, int x)	{ return tiles[y][x];																	}
		Texture*		getTexture()			{ return this->texture;																	}
		HeightMapVertex getVertex(int y, int x) { return tiles[y / TILE_SIZE][x / TILE_SIZE].getVertex(y % TILE_SIZE, x % TILE_SIZE);	}

		void useTexture(GLuint shaderProgram, GLuint textureUnit = GL_TEXTURE0);


		void render();

		~LargeTile();

	private:
		int x = 0, y = 0;

		Shader*  shaders		= nullptr;
		GLuint   shaderProgram	= 0;

		Texture* texture		= nullptr;
		//Texture* texture1		= nullptr;//

		GLuint VAO, VBO, IBO;

		Tile tiles[LARGETILE_ARR_SIZE][LARGETILE_ARR_SIZE];
		HeightMapVertex junction_vertices[2][LARGETILE_ARR_SIZE][LT_JUNCTION_VERTICES_SIZE][TILE_SIZE];//[axes y et x][nombre de lignes et de colonnes][nombre de côté pour les jointures (2 côté pour 1 jointure)][nombre de vertices par côté]
																		  
		std::vector<HeightMapVertex> v_vertices = {};
		std::vector<unsigned int>	 v_indices	= {};

		void setJunctions();
};


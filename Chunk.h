#pragma once
#include <GL/glew.h>
#include "uti.hpp"
#include "LargeTile.h"

class Chunk
{
public:
	Chunk(int x, int y, std::map<std::string, Shader>& shaders, LargeTile*** largesTiles = nullptr);

	//--- Getters ---//
	int getX() { return x; }
	int getY() { return y; }
	LargeTile* getLargeTile(int y, int x) { return largeTiles[y][x]; }

	//--- Setters ---//
	void setY(int y)			{ this->y = y;					}
	void setX(int x)			{ this->x = x;					}
	void setXY(int x, int y)	{ this->x = x;	this->y = y;	}

	void setJunctions();

	void render();

	~Chunk();

private:
	GLuint VAO, VBO, IBO;

	Shader* junction_shaders = nullptr;

	int x = 0, y = 0;
	LargeTile*** largeTiles = nullptr;
	HeightMapVertex junction_vertices[2][CHUNK_ARR_SIZE][CK_JUNCTION_VERTICES_SIZE][LARGETILE_SIZE];//[axes y et x][nombre de lignes et de colonnes][nombre de côté pour les jointures (2 côté pour 1 jointure)][nombre de vertices par côté]

	std::vector<HeightMapVertex> v_vertices = {};
	std::vector<unsigned int>	 v_indices = {};
};


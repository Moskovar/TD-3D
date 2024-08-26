#pragma once
#include <GL/glew.h>
#include "uti.hpp"
#include "LargeTile.h"

#define CHUNK_ARR_SIZE 2 //2x2

class Chunk
{
public:
	Chunk(int x, int y);

	//--- Getters ---//
	int getX() { return x; }
	int getY() { return y; }

	//--- Setters ---//
	void setY(int y)			{ this->y = y;					}
	void setX(int x)			{ this->x = x;					}
	void setXY(int x, int y)	{ this->x = x;	this->y = y;	}

	void render();

	~Chunk();

private:
	GLuint VAO, VBO, IBO;

	int x = 0, y = 0;
	LargeTile largeTiles[CHUNK_ARR_SIZE][CHUNK_ARR_SIZE];

	std::vector<HeightMapVertex> v_vertices = {};
	std::vector<unsigned int>	 v_indices = {};
};


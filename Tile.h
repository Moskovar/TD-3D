#pragma once
#include <vector>
#include "uti.hpp"

#define TILE_SIZE 32

class Tile
{
	public:
		Tile();
		//Tile(int x, int y, HeightMapVertex** vertices);

		//--- Getters ---//
		int getX()									{ return x;					}
		int getY()									{ return y;					}
		HeightMapVertex  getVertex(int y, int x)	{ return vertices[y][x];	}
		HeightMapVertex* getPVertex(int y, int x)	{ return &(vertices[y][x]); }
		std::vector<HeightMapVertex> getVertices()	{ return v_vertices;		}
		std::vector<unsigned int>	 getIndices()	{ return v_indices;			}

		//--- Setters ---//
		void setYX(int y, int x)								{ this->y = y;	this->x = x;		}
		void setVertices(HeightMapVertex** vertices)			{ this->vertices = vertices;		}
		void setVertexIndice(int y, int x, unsigned int indice) { vertices[y][x].indice = indice;	}
		void setVertex(int y, int x, HeightMapVertex vertex)	{ vertices[y][x] = vertex;			}
		void setVectors();//Met à jour les vecteurs de vertices et indices

		void render();

		~Tile();

	private:
		int x = 0, y = 0;

		GLuint VAO, VBO, IBO;

		HeightMapVertex**			 vertices	= nullptr;
		std::vector<HeightMapVertex> v_vertices = {};
		std::vector<unsigned int>	 v_indices	= {};
};


#pragma once
#include "Chunk.h"
namespace Game
{

	class Map
	{
		public:
			Map(std::map<std::string, Shader>& shaders, Chunk*** chunks = nullptr);

			Chunk* getChunk(int y, int x) { return chunks[y][x]; }

			void setChunk(int y, int x, Chunk* chunk) { this->chunks[y][x] = chunk; }
			void setJunctions();

			void render();

			~Map();

		private:
			GLuint VAO, VBO, IBO;

			Shader* junctions_shaders = nullptr;

			Chunk*** chunks = nullptr;
			HeightMapVertex junction_vertices[2][MAP_ARR_SIZE][MP_JUNCTION_VERTICES_SIZE][LARGETILE_SIZE * CHUNK_ARR_SIZE];//[axes y et x][nombre de lignes et de colonnes][nombre de côté pour les jointures (2 côté pour 1 jointure)][nombre de vertices par côté]

			std::vector<HeightMapVertex> v_vertices = {};
			std::vector<unsigned int>	 v_indices = {};
	};
}


#pragma once
#include "Chunk.h"
namespace Game
{

	class Map
	{
		public:
			Map();

			Chunk* getChunk(int y, int x) { return chunks[y][x]; }

			void render();

			~Map();

		private:
			GLuint VAO, VBO, IBO;

			Chunk*** chunks = nullptr;

	};
}


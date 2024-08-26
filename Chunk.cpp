#include "Chunk.h"

Chunk::Chunk(int x, int y)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
}

void Chunk::render()
{
	for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
		for (int x = 0; y < CHUNK_ARR_SIZE; ++x)
			largeTiles[y][x].render();
}

Chunk::~Chunk()
{
    if (IBO != 0)
    {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }

    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    printf("||--- Chunk cleared ---||\n");
}

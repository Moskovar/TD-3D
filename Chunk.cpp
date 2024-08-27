#include "Chunk.h"

Chunk::Chunk(int x, int y)
{
    this->y = y;
    this->x = x;

    largeTiles = new LargeTile * *[CHUNK_ARR_SIZE];

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
    {
        largeTiles[y] = new LargeTile * [CHUNK_ARR_SIZE];
        for (int x = 0; x < CHUNK_ARR_SIZE; ++x)
            largeTiles[y][x] = new LargeTile(y, x, this->y, this->x, "h1.exr", "h1.png");
    }

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);
}

void Chunk::render()
{
	for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
		for (int x = 0; x < CHUNK_ARR_SIZE; ++x)
            if (largeTiles[y][x])
                largeTiles[y][x]->render();
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

    if (largeTiles)
    {
        for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
        {
            for (int x = 0; x < CHUNK_ARR_SIZE; ++x)
            {
                delete largeTiles[y][x];
                largeTiles[y][x] = nullptr;
            }

            if (largeTiles[y])
            {
                delete largeTiles[y];
                largeTiles[y] = nullptr;
            }
        }

        delete largeTiles;
        largeTiles = nullptr;
    }

    printf("||--- Chunk cleared ---||\n");
}

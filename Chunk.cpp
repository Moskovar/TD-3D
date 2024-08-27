#include "Chunk.h"

Chunk::Chunk(int x, int y)
{
    largeTiles = new LargeTile * *[CHUNK_ARR_SIZE];

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
    {
        largeTiles[y] = new LargeTile * [CHUNK_ARR_SIZE];
        for (int x = 0; x < CHUNK_ARR_SIZE; ++x)
            largeTiles[y][x] = new LargeTile(y, x, "h1.exr", "h1.png");
    }

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
    {
        for (int x = 0; x < CHUNK_ARR_SIZE; ++x)
            if (largeTiles[y][x]) std::cout << "LT: " << largeTiles[y][x]->getVertex(0, 0).x << std::endl;
            
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

    printf("||--- Chunk cleared ---||\n");
}

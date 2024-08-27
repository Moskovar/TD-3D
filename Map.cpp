#include "Map.h"

Game::Map::Map()
{
	chunks = new Chunk * *[MAP_ARR_SIZE];

	for (int y = 0; y < MAP_ARR_SIZE; ++y)
    {
        chunks[y] = new Chunk * [MAP_ARR_SIZE];

        for (int x = 0; x < MAP_ARR_SIZE; ++x)
            chunks[y][x] = new Chunk(y, x);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
}

void Game::Map::render()
{
    for (int y = 0; y < MAP_ARR_SIZE; ++y)
        for (int x = 0; x < MAP_ARR_SIZE; ++x)
            if(chunks[y][x]) chunks[y][x]->render();
}

Game::Map::~Map()
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

    if (chunks)
    {
        for (int y = 0; y < MAP_ARR_SIZE; ++y)
        {
            for (int x = 0; x < MAP_ARR_SIZE; ++x)
            {
                delete chunks[y][x];
                chunks[y][x] = nullptr;
            }

            if (chunks[y])
            {
                delete chunks[y];
                chunks[y] = nullptr;
            }
        }

        delete chunks;
        chunks = nullptr;
    }

    printf("||--- Chunk cleared ---||\n");
}

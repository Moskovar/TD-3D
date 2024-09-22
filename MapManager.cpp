#include "MapManager.h"

void MapManager::loadMap(Game::Map* map, std::map<std::string, Shader>& shaders)
{
    //Loading chunks
    LargeTile*** largeTiles = new LargeTile * *[CHUNK_ARR_SIZE];

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
        largeTiles[y] = new LargeTile * [CHUNK_ARR_SIZE];

    //Chunk 0 0                                                                     
    largeTiles[0][0] = new LargeTile(0, 0, 0, 0, "00.exr", "h1.png", shaders);
    largeTiles[1][0] = new LargeTile(1, 0, 0, 0, "10.exr", "h2.png", shaders);
    largeTiles[0][1] = new LargeTile(0, 1, 0, 0, "01.exr", "h3.png", shaders);
    largeTiles[1][1] = new LargeTile(1, 1, 0, 0, "11.exr", "h4.png", shaders);

    map->setChunk(0, 0, new Chunk(0, 0, shaders, largeTiles));

    //Chunk 0 1
    LargeTile*** largeTiles2 = new LargeTile * *[CHUNK_ARR_SIZE];

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
        largeTiles2[y] = new LargeTile * [CHUNK_ARR_SIZE];

    largeTiles2[0][0] = new LargeTile(0, 0, 0, 1, "flat.exr", "h1.png", shaders);
    largeTiles2[1][0] = new LargeTile(1, 0, 0, 1, "flat.exr", "h2.png", shaders);
    largeTiles2[0][1] = new LargeTile(0, 1, 0, 1, "flat.exr", "h3.png", shaders);
    largeTiles2[1][1] = new LargeTile(1, 1, 0, 1, "flat.exr", "h4.png", shaders);

    map->setChunk(0, 1, new Chunk(0, 1, shaders, largeTiles2));

    //Chunk 1 0
    LargeTile*** largeTiles3 = new LargeTile * *[CHUNK_ARR_SIZE];

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
        largeTiles3[y] = new LargeTile * [CHUNK_ARR_SIZE];

    largeTiles3[0][0] = new LargeTile(0, 0, 1, 0, "flat.exr", "h1.png", shaders);
    largeTiles3[1][0] = new LargeTile(1, 0, 1, 0, "flat.exr", "h2.png", shaders);
    largeTiles3[0][1] = new LargeTile(0, 1, 1, 0, "flat.exr", "h3.png", shaders);
    largeTiles3[1][1] = new LargeTile(1, 1, 1, 0, "flat.exr", "h4.png", shaders);

    map->setChunk(1, 0, new Chunk(1, 0, shaders, largeTiles3));

    LargeTile*** largeTiles4 = new LargeTile * *[CHUNK_ARR_SIZE];

    for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
        largeTiles4[y] = new LargeTile * [CHUNK_ARR_SIZE];

    largeTiles4[0][0] = new LargeTile(0, 0, 1, 1, "flat.exr", "h1.png", shaders);
    largeTiles4[1][0] = new LargeTile(1, 0, 1, 1, "flat.exr", "h2.png", shaders);
    largeTiles4[0][1] = new LargeTile(0, 1, 1, 1, "flat.exr", "h3.png", shaders);
    largeTiles4[1][1] = new LargeTile(1, 1, 1, 1, "flat.exr", "h4.png", shaders);

    map->setChunk(1, 1, new Chunk(1, 1, shaders, largeTiles4));
}
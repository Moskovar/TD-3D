#include "LargeTile.h"

LargeTile::LargeTile(int x, int y, const char* heightmapPath)
{
	this->x = x;
	this->y = y;

    // Charger l'image PNG
    int imgWidth, imgHeight, channels;
    unsigned char* heightmapData = stbi_load(heightmapPath, &imgWidth, &imgHeight, &channels, 1); // 1 pour charger en niveau de gris

    if (!heightmapData)
    {
        std::cerr << "Erreur de chargement de l'image heightmap." << std::endl;
        return;
    }

    // Vérifiez que la largeur et la hauteur de l'image correspondent à la taille attendue
    if (imgWidth != LARGETILE_SIZE || imgHeight != LARGETILE_SIZE) {
        std::cerr << "Dimensions de l'image heightmap ne correspondent pas." << std::endl;
        stbi_image_free(heightmapData);
        return;
    }

    unsigned int indice = 0,//indice du vertex
                 cy     = 0,//coordonnée y de la Tile dans le tableau de la LargeTile
                 cx     = 0;//coordonnée x de la Tile dans le tableau de la LargeTile
    // Générer le maillage de terrain
    int size = LARGETILE_ARR_SIZE * LARGETILE_ARR_SIZE;//Nombre de tiles dans le tableau (ex: 16 x 16)
    for (int i = 0; i < size; i++)
    {
        tiles[cy][cx].setYX(cy, cx);
        for (int y = 0; y < TILE_SIZE; ++y)//on parcourt chaque pixel de la heightmap par carré de 32x32 pour remplir les Tiles
        {
            for (int x = 0; x < TILE_SIZE; ++x)
            {
                GLfloat heightValue = heightmapData[y + (cy * TILE_SIZE) * TILE_SIZE + x + (cx * TILE_SIZE)];
                //std::cout << (GLfloat)y + (GLfloat)(cy * TILE_SIZE) << std::endl;
                tiles[cy][cx].setVertex(y % TILE_SIZE, x % TILE_SIZE, { (GLfloat)x + (GLfloat)(cx * TILE_SIZE), heightValue / 255.0f * MAX_HEIGHT, (GLfloat)y + (GLfloat)(cy * TILE_SIZE), indice});
                indice++;
            }
        }

        cx++;
        if (cx == 16)
        {
            cy++;
            cx = 0;
        }

        //tiles[cy][cx].setVectors();

        indice = 0;
    }

    //std::cout << "Vertex indice: " << tiles[1][0].getVertex(0, 0).indice << std::endl;

    //for (int y = 0; y < LARGETILE_ARR_SIZE; ++y)
    //    for (int x = 0; x < LARGETILE_ARR_SIZE; ++x)
    //        std::cout << "YX: " << tiles[y][x].getY() << " ... " << tiles[y][x].getX() << std::endl;

    // Libérer la mémoire de l'image
    if (heightmapData)
        stbi_image_free(heightmapData);
}

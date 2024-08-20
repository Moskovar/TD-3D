#include "LargeTile.h"



LargeTile::LargeTile(const char* heightmapPath, int x, int y)
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

    // V�rifiez que la largeur et la hauteur de l'image correspondent � la taille attendue
    if (imgWidth != LARGETILE_SIZE || imgHeight != LARGETILE_SIZE) {
        std::cerr << "Dimensions de l'image heightmap ne correspondent pas." << std::endl;
        stbi_image_free(heightmapData);
        return;
    }

    unsigned int indice = 0;
    // G�n�rer le maillage de terrain
    for (int y = 0; y < LARGETILE_SIZE; ++y) {
        for (int x = 0; x < LARGETILE_SIZE; ++x) {
            // Lire la valeur de hauteur du tableau heightmapData
            GLfloat heightValue = heightmapData[y * LARGETILE_SIZE + x];
            //std::cout << heightValue << std::endl;
            GLfloat inGameMaxHeight = 100.0f;
            // D�finir la position du vertex en fonction de la hauteur
            //vertices[y][x] = { (GLfloat)x, heightValue / 255.0f * inGameMaxHeight, (GLfloat)y, indice };
            indice++;
        }
    }

    // Lib�rer la m�moire de l'image
    if (heightmapData)
        stbi_image_free(heightmapData);
}

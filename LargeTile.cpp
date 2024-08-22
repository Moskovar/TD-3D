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
                heightValue = 0;
                //std::cout << (GLfloat)y + (GLfloat)(cy * TILE_SIZE) << std::endl;
                tiles[cy][cx].setVertex(y % TILE_SIZE, x % TILE_SIZE, { (GLfloat)x + (GLfloat)(cx * TILE_SIZE), heightValue / 255.0f * MAX_HEIGHT, (GLfloat)y + (GLfloat)(cy * TILE_SIZE), indice});
                indice++;
            }
        }

        tiles[cy][cx].setVectors();

        cx++;
        if (cx == 16)
        {
            cy++;
            cx = 0;
        }

        indice = 0;
    }

    setBuffers();

    //std::cout << "Vertex indice: " << tiles[1][0].getVertex(0, 0).indice << std::endl;

    //for (int y = 0; y < LARGETILE_ARR_SIZE; ++y)
    //    for (int x = 0; x < LARGETILE_ARR_SIZE; ++x)
    //        std::cout << "YX: " << tiles[y][x].getY() << " ... " << tiles[y][x].getX() << std::endl;

    // Libérer la mémoire de l'image
    if (heightmapData)
        stbi_image_free(heightmapData);
}

void LargeTile::setBuffers()
{
    //Récupération des vertices qui forment les jointures à partir des tuiles
    for(int y = 0; y < 2 * (TILE_SIZE - 2) - 1; y += 2)
        for (int x = 0; x < TILE_SIZE; ++x)
        {
            junction_vertices[y][x] = tiles[0][0].getVertex(31, x);
            junction_vertices[y][x].indice = (y * 32) + x;

            junction_vertices[y + 1][x] = tiles[1][0].getVertex(0, x);
            junction_vertices[y + 1][x].indice = ((y + 1) * 32) + x;
        }

    //Ajout de chaque vertex des jointures au vertices à envoyer à opengl
    for (int y = 0; y < 2; ++y)
        for (int x = 0; x < TILE_SIZE; ++x)
            v_vertices.push_back(junction_vertices[y][x]);

    //Ajout des indices pour former les triangles à envoyer à opengl
    for (int x = 0; x < TILE_SIZE - 1; ++x)
    {
        //triangle de droite
        v_indices.push_back(junction_vertices[0][x].indice);
        v_indices.push_back(junction_vertices[0][x + 1].indice);
        v_indices.push_back(junction_vertices[1][x].indice);

        //triangle de gauche
        v_indices.push_back(junction_vertices[1][x].indice);
        v_indices.push_back(junction_vertices[0][x + 1].indice);
        v_indices.push_back(junction_vertices[1][x + 1].indice);
    }
   
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v_vertices.size() * sizeof(HeightMapVertex), v_vertices.data(), GL_STATIC_DRAW);

    // Lier et remplir l'EBO (Element Buffer Object)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, v_indices.size() * sizeof(unsigned int), v_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HeightMapVertex), (void*)offsetof(HeightMapVertex, x));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LargeTile::render()
{
    for (int y = 0; y < LARGETILE_ARR_SIZE; ++y)
        for (int x = 0; x < LARGETILE_ARR_SIZE; ++x)
            tiles[y][x].render();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, v_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

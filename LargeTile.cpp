#include "LargeTile.h"

LargeTile::LargeTile(int x, int y, const char* heightmapPath)
{
	this->x = x;
	this->y = y;

    // Charger l'image PNG
    //int imgWidth, imgHeight, channels;
    //unsigned char* heightmapData = stbi_load(heightmapPath, &imgWidth, &imgHeight, &channels, 1); // 1 pour charger en niveau de gris

    //if (!heightmapData)
    //{
    //    std::cerr << "Erreur de chargement de l'image heightmap." << std::endl;
    //    return;
    //}

    //// Vérifiez que la largeur et la hauteur de l'image correspondent à la taille attendue
    //if (imgWidth != LARGETILE_SIZE || imgHeight != LARGETILE_SIZE) {
    //    std::cerr << "Dimensions de l'image heightmap ne correspondent pas." << std::endl;
    //    stbi_image_free(heightmapData);
    //    return;
    //}

    //unsigned int indice = 0,//indice du vertex
    //             cy     = 0,//coordonnée y de la Tile dans le tableau de la LargeTile
    //             cx     = 0;//coordonnée x de la Tile dans le tableau de la LargeTile
    //// Générer le maillage de terrain
    //int size = LARGETILE_ARR_SIZE * LARGETILE_ARR_SIZE;//Nombre de tiles dans le tableau (ex: 16 x 16)
    //for (int i = 0; i < size; i++)
    //{
    //    tiles[cy][cx].setYX(cy, cx);
    //    for (int y = 0; y < TILE_SIZE; ++y)//on parcourt chaque pixel de la heightmap par carré de 32x32 pour remplir les Tiles
    //    {
    //        for (int x = 0; x < TILE_SIZE; ++x)
    //        {
    //            GLfloat heightValue = heightmapData[y + (cy * TILE_SIZE) * TILE_SIZE + x + (cx * TILE_SIZE)];
    //            //heightValue = 0;
    //            //std::cout << (GLfloat)y + (GLfloat)(cy * TILE_SIZE) << std::endl;
    //            tiles[cy][cx].setVertex(y % TILE_SIZE, x % TILE_SIZE, { (GLfloat)x + (GLfloat)(cx * TILE_SIZE), heightValue / 255.0f * MAX_HEIGHT, (GLfloat)y + (GLfloat)(cy * TILE_SIZE), indice});
    //            indice++;
    //        }
    //    }

    //    tiles[cy][cx].setVectors();

    //    cx++;
    //    if (cx == 16)
    //    {
    //        cy++;
    //        cx = 0;
    //    }

    //    indice = 0;
    //}


    const char* err = NULL;
    int width, height;
    float* image;

    int ret = IsEXR(heightmapPath);
    if (ret != TINYEXR_SUCCESS)
    {
        fprintf(stderr, "File not found or given file is not a EXR format. code %d\n", ret);
        exit(-1);
    }
    else std::cout << "ok its exr file" << std::endl;

    ret = LoadEXR(&image, &width, &height, heightmapPath, &err);
    if (ret != TINYEXR_SUCCESS) {
        if (err)
        {
            fprintf(stderr, "Load EXR err: %s(code %d)\n", err, ret);
        }
        else
        {
            fprintf(stderr, "Load EXR err: code = %d\n", ret);
        }
        FreeEXRErrorMessage(err);
        return;
    }
    else std::cout << "Success loading exr" << std::endl;


    if (image != nullptr) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int idx = (height - y - 1) * width + x;
                float r = image[4 * idx + 0];  // Utilisez le canal rouge si l'image est en RGBA

                //if(r > 0.1) std::cout << r << std::endl;
            }
        }
    }
    else {
        std::cerr << "Error: Image data is null." << std::endl;
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
                // Calcul de l'index dans l'image globale
                int globalX = x + cx * TILE_SIZE;
                int globalY = y + cy * TILE_SIZE;
                int pixelIdx = (globalY * LARGETILE_SIZE + globalX) * 4; // 4 pour RGBA
                GLfloat heightValue = image[pixelIdx];
                ////heightValue = 0;
                ////std::cout << (GLfloat)y + (GLfloat)(cy * TILE_SIZE) << std::endl;
                if (heightValue > 0.1) std::cout << heightValue << std::endl;
                ////if(heightValue > 0.01) std::cout << heightValue * 255 << std::endl;
                tiles[cy][cx].setVertex(y % TILE_SIZE, x % TILE_SIZE, { (GLfloat)x + (GLfloat)(cx * TILE_SIZE), heightValue * MAX_HEIGHT, (GLfloat)y + (GLfloat)(cy * TILE_SIZE), indice});
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
    //if (heightmapData)
    //    stbi_image_free(heightmapData);
    free(image);
}

void LargeTile::setBuffers()
{
    unsigned int indice = 0;
    for(int i = 0; i < 2; ++i)//0 = jointures y sur x, 1 = jointures x sur y
    {
        for (int j = 0; j < LARGETILE_ARR_SIZE; ++j)//On parcourt chaque rangée de tuiles
        {
            for (int y = 0; y < JUNCTION_VERTICES_SIZE - 1; y += 2)//On parcourt chaque jointure (une jointure = 2 rangées de vertices)
            {
                for (int x = 0; x < TILE_SIZE; ++x)//On parcourt chaque vertex
                {
                    //--- Récupération des pointeurs HeightMapVertex pour modification ---//
                    HeightMapVertex *vertex1 = nullptr, *vertex2 = nullptr;

                    if (i == 0)
                    {
                        vertex1 = tiles[ y / 2     ][j].getPVertex(31, x);
                        vertex2 = tiles[(y / 2) + 1][j].getPVertex(0, x);
                    }
                    else if (i == 1)
                    {
                        vertex1 = tiles[j][y / 2].getPVertex(x, 31);
                        vertex2 = tiles[j][(y / 2) + 1].getPVertex(x, 0);
                    }

                    vertex1->indice = indice++;
                    vertex2->indice = indice++;

                    junction_vertices[i][j][y + 1][x] = *vertex1;
                    junction_vertices[i][j][y    ][x] = *vertex2;

                    v_vertices.push_back(*vertex1);
                    v_vertices.push_back(*vertex2);
                }

                for (int x = 0; x < TILE_SIZE - 1; ++x)//On récupère les indices pour l'IBO
                {
                    //triangle de droite
                    v_indices.push_back(junction_vertices[i][j][y][x].indice);
                    v_indices.push_back(junction_vertices[i][j][y][x + 1].indice);
                    v_indices.push_back(junction_vertices[i][j][y + 1][x].indice);

                    //triangle de gauche                  
                    v_indices.push_back(junction_vertices[i][j][y + 1][x].indice);
                    v_indices.push_back(junction_vertices[i][j][y][x + 1].indice);
                    v_indices.push_back(junction_vertices[i][j][y + 1][x + 1].indice);
                }
            }
        }
    }

    //--- On fait le joint du coin de chaque 4 tuiles ---//
    for(int x = 0; x < LARGETILE_ARR_SIZE - 1; ++x)//On parcourt les tuiles sur x
    {
        for (int y = 0; y < JUNCTION_VERTICES_SIZE - 1; y += 2)//On parcourt les joints sur y (un joint = 2 rangée de vertices)
        {
            //triangle de droite
            v_indices.push_back(junction_vertices[0][x][y + 1][31].indice);
            v_indices.push_back(junction_vertices[0][x][y][31].indice);
            v_indices.push_back(junction_vertices[0][x + 1][y][0].indice);

            //triangle de gauche                  
            v_indices.push_back(junction_vertices[0][x][y + 1][31].indice);
            v_indices.push_back(junction_vertices[0][x + 1][y + 1][0].indice);
            v_indices.push_back(junction_vertices[0][x + 1][y][0].indice);
        }
    }

    //for(int i = 0; i < 2; ++i)
    //{
    //    for (int j = 0; j < LARGETILE_ARR_SIZE; ++j)
    //    {
    //        for (int y = 0; y < JUNCTION_VERTICES_SIZE - 1; y += 2);
    //    }      
    //}

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

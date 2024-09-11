#include "LargeTile.h"

LargeTile::LargeTile(int y, int x, int yChunk, int xChunk, std::string heightmapName, std::string textureName)
{
	this->y = y;// * (yChunk + 1);
	this->x = x;// * (xChunk + 1);

    int largeTileGlobalY = y * LARGETILE_SIZE + yChunk * CHUNK_ARR_SIZE * LARGETILE_SIZE,//On ajoute le offset du chunk dans la map également
        largeTileGlobalX = x * LARGETILE_SIZE + xChunk * CHUNK_ARR_SIZE * LARGETILE_SIZE;

    std::cout << " y: " << y << " ... " << "x: " << x << std::endl;

    if(y % 2 == 0 && x % 2 == 0 || y % 2 == 1 && x % 2 == 1)  
    {
        this->texture = new Texture("textures/" + textureName);
        printf("h1 loaded !\n");
    }
    else            
    {
        this->texture = new Texture("textures/" + std::string("h2.png"));
        printf("h2 loaded !\n");
    }

    const char* err = NULL;
    int width, height;
    float* image;
    std::string heightmapPath = "heightmaps/" + heightmapName;

    int ret = IsEXR(heightmapPath.c_str());
    if (ret != TINYEXR_SUCCESS)
    {
        fprintf(stderr, "File [ %s ] not found or given file is not a EXR format. code %d\n", heightmapPath.c_str(), ret);
        exit(-1);
    }
    else std::cout << "ok its exr file" << std::endl;

    

    ret = LoadEXR(&image, &width, &height, heightmapPath.c_str(), &err);
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
                int globalY = y + cy * TILE_SIZE,
                    globalX = x + cx * TILE_SIZE;
                int pixelIdx = (globalY * LARGETILE_SIZE + globalX) * 4; // 4 pour RGBA
                GLfloat heightValue = image[pixelIdx];
                //heightValue = 0.0f;
                //à améliorer c'est carrément bancale mais ça dépanne le temps
                if(globalX == 0 || globalY == 0)//à vérifier dynamiquement au chargement des Larges tuiles
                {
                    short count = 0;

                    // Vérifier le pixel au-dessus (y-1)
                    if (y > 0 && image[((y - 1) * width + x) * 4] > 0.0f)           ++count;
 
                    // Vérifier le pixel en dessous (y+1)
                    if (y < height - 1 && image[((y + 1) * width + x) * 4] > 0.0f)  ++count;

                    // Vérifier le pixel à gauche (x-1)
                    if (x > 0 && image[(y * width + (x - 1)) * 4] > 0.0f)           ++count;

                    // Vérifier le pixel à droite (x+1)
                    if (x < width - 1 && image[(y * width + (x + 1)) * 4] > 0.0f)   ++count;

                    if (count <= 2) heightValue = 0.0f;
                }


                if (heightValue < 0.01f) heightValue = 0.0f;//pour éliminer les résidus ?? pas propre ...

                glm::vec2 texCoords((GLfloat)globalX / (GLfloat)(LARGETILE_SIZE - 1), (GLfloat)globalY / (GLfloat)(LARGETILE_SIZE - 1));//Attribution des coordonnées de textures

                globalY += largeTileGlobalY;//ajout du offset de la LargeTile
                globalX += largeTileGlobalX;//Après récup de la heightValue car la heightValue se situe dans la heightmap de la LargeTile et pas dans le chunk

                tiles[cy][cx].setVertex(y % TILE_SIZE, x % TILE_SIZE, { (GLfloat) globalX, heightValue * MAX_HEIGHT, (GLfloat)globalY, indice, texCoords});
                indice++;
            }
        }
    
        tiles[cy][cx].setIndices();
    
        cx++;
        if (cx == 16)
        {
            cy++;
            cx = 0;
        }
    
        indice = 0;
    }

    setJunctions();

    free(image);
}

void LargeTile::setJunctions()
{
    unsigned int indice = 0;
    for(int i = 0; i < 2; ++i)//0 = jointures y sur x, 1 = jointures x sur y
    {
        for (int j = 0; j < LARGETILE_ARR_SIZE; ++j)//On parcourt chaque rangée de tuiles
        {
            for (int y = 0; y < LT_JUNCTION_VERTICES_SIZE - 1; y += 2)//On parcourt chaque jointure (une jointure = 2 rangées de vertices)
            {
                for (int x = 0; x < TILE_SIZE; ++x)//On parcourt chaque vertex
                {
                    //--- Récupération des pointeurs HeightMapVertex pour modification ---//
                    HeightMapVertex *vertex1 = nullptr, *vertex2 = nullptr;

                    if (i == 0)
                    {
                        vertex1 = tiles[ y / 2     ][j].getPVertex(TILE_SIZE - 1, x);
                        vertex2 = tiles[(y / 2) + 1][j].getPVertex(0, x);
                    }
                    else if (i == 1)
                    {
                        vertex1 = tiles[j][y / 2].getPVertex(x, TILE_SIZE - 1);
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
        for (int y = 0; y < LT_JUNCTION_VERTICES_SIZE - 1; y += 2)//On parcourt les joints sur y (un joint = 2 rangée de vertices)
        {
            //triangle de droite
            v_indices.push_back(junction_vertices[0][x][y + 1][TILE_SIZE - 1].indice);
            v_indices.push_back(junction_vertices[0][x][y][TILE_SIZE - 1].indice);
            v_indices.push_back(junction_vertices[0][x + 1][y][0].indice);

            //triangle de gauche                  
            v_indices.push_back(junction_vertices[0][x][y + 1][TILE_SIZE - 1].indice);
            v_indices.push_back(junction_vertices[0][x + 1][y + 1][0].indice);
            v_indices.push_back(junction_vertices[0][x + 1][y][0].indice);
        }
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

    // Attribut des coordonnées de texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(HeightMapVertex), (void*)offsetof(HeightMapVertex, texCoords));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LargeTile::render()
{
    if(texture) texture->useTexture();

    for (int y = 0; y < LARGETILE_ARR_SIZE; ++y)
        for (int x = 0; x < LARGETILE_ARR_SIZE; ++x)
            tiles[y][x].render();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, v_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

LargeTile::~LargeTile()
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

    if(texture)
    {
        delete texture;
        texture = nullptr;
    }

    printf("||--- LargeTile cleared ---||\n");
}

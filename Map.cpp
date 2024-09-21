#include "Map.h"

Game::Map::Map(std::map<std::string, Shader>& shaders, Chunk*** chunks)
{
    this->junctions_shaders = &shaders[MAP_JUNCTIONS_SHADERS];

    if (!chunks)
    {
        this->chunks = new Chunk * *[MAP_ARR_SIZE];

        for (int y = 0; y < MAP_ARR_SIZE; ++y)
        {
            this->chunks[y] = new Chunk * [MAP_ARR_SIZE];

            for (int x = 0; x < MAP_ARR_SIZE; ++x)
                this->chunks[y][x] = new Chunk(y, x, shaders);
        }
    }
    else this->chunks = chunks;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    setJunctions();
}

void Game::Map::setJunctions()
{
    int chunkVertices = LARGETILE_SIZE * CHUNK_ARR_SIZE; //nombre devertices dans un chunk (nombre de LargeTiles * nb vertices par LargeTile)

    unsigned int indice = 0;
    for (int i = 0; i < 2; ++i)//0 = jointures y sur x, 1 = jointures x sur y
    {
        for (int j = 0; j < MAP_ARR_SIZE; ++j)//On parcourt chaque rangée de chunk
        {
            for (int y = 0; y < MP_JUNCTION_VERTICES_SIZE - 1; y += 2)//On parcourt chaque jointure (une jointure = 2 rangées de vertices)
            {
                //--- Récupération des pointeurs HeightMapVertex pour modification ---//
                HeightMapVertex vertex1, vertex2;

                for (int x = 0; x < chunkVertices; ++x)//on parcourt tous les vertices d'un chunk (nombre de LargeTiles * nb vertices par LargeTile)
                {
                    if (i == 0)
                    {
                        vertex1 = chunks[j][ y / 2     ]->getLargeTile(CHUNK_ARR_SIZE - 1, x / LARGETILE_SIZE)->getVertex(LARGETILE_SIZE - 1, x % LARGETILE_SIZE);
                        vertex2 = chunks[j][(y / 2) + 1]->getLargeTile(0                 , x / LARGETILE_SIZE)->getVertex(0                 , x % LARGETILE_SIZE);
                        //std::cout << vertex1.x << " ... " << vertex2.x << std::endl;
                        vertex1.indice = indice++;
                        vertex2.indice = indice++;

                        junction_vertices[i][j][y + 1][x] = vertex1;
                        junction_vertices[i][j][y][x] = vertex2;

                        v_vertices.push_back(vertex1);
                        v_vertices.push_back(vertex2);
                    }
                    else if (i == 1)
                    {
                        vertex1 = chunks[ y / 2     ][j]->getLargeTile(x / LARGETILE_SIZE, CHUNK_ARR_SIZE - 1)->getVertex(x % LARGETILE_SIZE, LARGETILE_SIZE - 1);
                        vertex2 = chunks[(y / 2) + 1][j]->getLargeTile(x / LARGETILE_SIZE, 0                 )->getVertex(x % LARGETILE_SIZE, 0                 );
                    }

                    vertex1.indice = indice++;
                    vertex2.indice = indice++;

                    junction_vertices[i][j][y + 1][x] = vertex1;
                    junction_vertices[i][j][y    ][x] = vertex2;

                    v_vertices.push_back(vertex1);
                    v_vertices.push_back(vertex2);
                }

                for (int x = 0; x < chunkVertices - 1; ++x)//On récupère les indices pour l'IBO
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

    //--- On fait le joint du coin de chaque 4 chunks ---//
    for (int x = 0; x < CHUNK_ARR_SIZE - 1; ++x)//On parcourt les tuiles sur x
    {
        for (int y = 0; y < CK_JUNCTION_VERTICES_SIZE - 1; y += 2)//On parcourt les joints sur y (un joint = 2 rangée de vertices)
        {
            //triangle de droite
            v_indices.push_back(junction_vertices[0][x][y + 1][chunkVertices - 1].indice);
            v_indices.push_back(junction_vertices[0][x][y][chunkVertices - 1].indice);
            v_indices.push_back(junction_vertices[0][x + 1][y][0].indice);

            //triangle de gauche                  
            v_indices.push_back(junction_vertices[0][x][y + 1][chunkVertices - 1].indice);
            v_indices.push_back(junction_vertices[0][x + 1][y + 1][0].indice);
            v_indices.push_back(junction_vertices[0][x + 1][y][0].indice);
        }
    }

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

void Game::Map::render()
{
    for (int y = 0; y < MAP_ARR_SIZE; ++y)
        for (int x = 0; x < MAP_ARR_SIZE; ++x)
            if(chunks[y][x]) chunks[y][x]->render();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, v_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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

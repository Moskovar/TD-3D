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

    setJunctions();
}

void Chunk::setJunctions()
{
    unsigned int indice = 0;
    for (int i = 0; i < 2; ++i)//0 = jointures y sur x, 1 = jointures x sur y
    {
        for (int j = 0; j < CHUNK_ARR_SIZE; ++j)//On parcourt chaque rangée de chunk
        {
            for (int y = 0; y < CK_JUNCTION_VERTICES_SIZE - 1; y += 2)//On parcourt chaque jointure (une jointure = 2 rangées de vertices)
            {
                for (int x = 0; x < LARGETILE_SIZE; ++x)//On parcourt chaque vertex
                {
                    //--- Récupération des pointeurs HeightMapVertex pour modification ---//
                    HeightMapVertex vertex1, vertex2;

                    if (i == 0)
                    {
                        vertex1 = largeTiles[y / 2][j]->getVertex(LARGETILE_SIZE - 1, x);
                        vertex2 = largeTiles[(y / 2) + 1][j]->getVertex(0, x);
                    }
                    else if (i == 1)
                    {
                        vertex1 = largeTiles[j][y / 2]->getVertex(x, LARGETILE_SIZE - 1);
                        vertex2 = largeTiles[j][(y / 2) + 1]->getVertex(x, 0);
                    }

                    vertex1.indice = indice++;
                    vertex2.indice = indice++;

                    junction_vertices[i][j][y + 1][x] = vertex1;
                    junction_vertices[i][j][y    ][x] = vertex2;

                    v_vertices.push_back(vertex1);
                    v_vertices.push_back(vertex2);
                }

                for (int x = 0; x < LARGETILE_SIZE - 1; ++x)//On récupère les indices pour l'IBO
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
    for (int x = 0; x < CHUNK_ARR_SIZE - 1; ++x)//On parcourt les tuiles sur x
    {
        for (int y = 0; y < CK_JUNCTION_VERTICES_SIZE - 1; y += 2)//On parcourt les joints sur y (un joint = 2 rangée de vertices)
        {
            //triangle de droite
            v_indices.push_back(junction_vertices[0][x][y + 1][LARGETILE_SIZE - 1].indice);
            v_indices.push_back(junction_vertices[0][x][y][LARGETILE_SIZE - 1].indice);
            v_indices.push_back(junction_vertices[0][x + 1][y][0].indice);

            //triangle de gauche                  
            v_indices.push_back(junction_vertices[0][x][y + 1][LARGETILE_SIZE - 1].indice);
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

void Chunk::render()
{
	for (int y = 0; y < CHUNK_ARR_SIZE; ++y)
		for (int x = 0; x < CHUNK_ARR_SIZE; ++x)
            if (largeTiles[y][x])
                largeTiles[y][x]->render();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, v_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
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

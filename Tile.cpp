#include "Tile.h"
#include <iostream>

Tile::Tile()
{
	this->vertices = new HeightMapVertex * [TILE_SIZE];

	for (int y = 0; y < TILE_SIZE; ++y)
		vertices[y] = new HeightMapVertex[TILE_SIZE];

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
}

void Tile::setIndices()
{
    for (int y = 0; y < TILE_SIZE; ++y)
    {
        for (int x = 0; x < TILE_SIZE; ++x)
        {
            //std::cout << y << " ... " << x << std::endl;
            v_vertices.push_back(vertices[y][x]);
            if (x < TILE_SIZE - 1 && y < TILE_SIZE - 1)
            {
                //triangle de gauche
                v_indices.push_back(vertices[y][x].indice);
                v_indices.push_back(vertices[y][x + 1].indice);
                v_indices.push_back(vertices[y + 1][x].indice);
            }

            if (x > 0 && y < TILE_SIZE - 1)
            {
                //triangle de droite
                v_indices.push_back(vertices[y][x].indice);
                v_indices.push_back(vertices[y + 1][x].indice);
                v_indices.push_back(vertices[y + 1][x - 1].indice);
            }
        }
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, v_vertices.size() * sizeof(HeightMapVertex), v_vertices.data(), GL_STATIC_DRAW);

    // Lier et remplir l'EBO (Element Buffer Object)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, v_indices.size() * sizeof(unsigned int), v_indices.data(), GL_STATIC_DRAW);

    // Attribut des coordonnées des vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HeightMapVertex), (void*)offsetof(HeightMapVertex, x));
    glEnableVertexAttribArray(0);

    // Attribut des coordonnées de texture
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(HeightMapVertex), (void*)offsetof(HeightMapVertex, texCoords));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Tile::render()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, v_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Tile::~Tile()
{
	if (vertices)
	{
		delete vertices;
		vertices = nullptr;
	}

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

    if (vertices)
    {
        for (int y = 0; y < TILE_SIZE; ++y)
        {
            if (vertices[y])
            {
                delete vertices[y];
                vertices[y] = nullptr;
            }
        }

        delete vertices;
        vertices = nullptr;
    }

	printf("||--- Tile cleared ---||\n");
}

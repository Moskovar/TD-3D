#include "Mesh.h"
#include <GLM/gtc/type_ptr.hpp>


Mesh::Mesh(std::vector<Vertex*> v_vertices, std::vector<unsigned int> v_indices)
{
	for (Vertex* vertex : v_vertices)
	{
		v_vertices_default.push_back({ vertex->id, vertex->position, vertex->normal, vertex->texCoords });
	}
	this->v_vertices = v_vertices;
	this->v_indices  = v_indices;
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;

	updateMesh();
}

void Mesh::updateMesh()
{
	std::vector<GLfloat> vertices2;
	for (Vertex* vertex : this->v_vertices)
	{
		vertices2.push_back(vertex->position.x);
		vertices2.push_back(vertex->position.y);
		vertices2.push_back(vertex->position.z);
								  
		vertices2.push_back(vertex->normal.x);
		vertices2.push_back(vertex->normal.y);
		vertices2.push_back(vertex->normal.z);
								  
		vertices2.push_back(vertex->texCoords.x);
		vertices2.push_back(vertex->texCoords.y);
	}

	GLfloat* vertices = &vertices2[0];
	unsigned int numOfVertices = vertices2.size();

	unsigned int* indices = &v_indices[0];
	unsigned int numOfIndices= v_indices.size();
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	resetMesh();
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
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

	indexCount = 0;

	for (Vertex* vertex : v_vertices)
	{
		if (vertex)
		{
			delete vertex;
			vertex = nullptr;
		}
	}
}

void Mesh::resetMesh()
{
	for (int i = 0; i < v_vertices.size(); ++i)
	{
		v_vertices[i]->position  = v_vertices_default[i].position;
		v_vertices[i]->normal    = v_vertices_default[i].normal;
		v_vertices[i]->texCoords = v_vertices_default[i].texCoords;
	}
}


Mesh::~Mesh()
{
	ClearMesh();
}

#pragma once
#include <GL/glew.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <vector>

struct Vertex
{
	unsigned int id = 0;
	//float weight    = 0.0f;
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;

	bool operator==(const Vertex& other) const
	{
		return id == other.id;
	}
};

class Mesh
{
	public:
		Mesh(std::vector<Vertex*> v_vertices, std::vector<unsigned int> indices);

		void updateMesh();
		void RenderMesh(const GLuint& modelLoc, glm::mat4 model);
		void ClearMesh();
		void resetMesh();

		~Mesh();

	private:
		GLuint VAO, VBO, IBO;
		GLsizei indexCount;
		std::vector<Vertex*> v_vertices;//contient tous les vertices de la mesh -> attribué dans createMesh
		std::vector<Vertex>  v_vertices_default;//contient tous les vertices de la mesh -> attribué dans createMesh
		std::vector<unsigned int> v_indices;//à voir à quoi ça correspond

};


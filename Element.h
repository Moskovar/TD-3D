#pragma once
#include "Model.h"

#define NUM_BONES  100

class Element
{
	public:
		Element() {};
		Element(short id, glm::vec3 position, const std::string& filePath);

		~Element();

		Model* getModel() { return model; }
		glm::vec3* getPositionP() { return &position; }
		
		void addX(GLfloat x) { position.x += x; }
		void addY(GLfloat y) { position.y += y; }
		void addZ(GLfloat z) { position.z += z; }
		void render(GLuint& modelLoc, GLuint& bonesTransformsLoc, float& animationTime, int idLoc);

	private:
		short id = -1;
		Model*    model       = nullptr;
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 position	  = glm::vec3(0.0f, 0.0f, 0.0f);
};


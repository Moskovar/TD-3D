#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <iostream>

using namespace std;

class Camera
{
	public:
		Camera(glm::vec3* target);

		glm::mat4* getViewMatrixP() { return &viewMatrix; }
		void setRadius(GLfloat radius) { this->radius = radius; }

		void mouseControl(GLFWwindow* window, GLfloat xChange, GLfloat yChange, GLfloat& scrollValue);

	private:
		glm::mat4  viewMatrix;
		glm::vec3  position, front, right, up, worldUp;
		glm::vec3* target;
		GLfloat yaw = -90.0f, pitch = 0.0f, radius = 30.0f, sensitivity = 0.5f;

		void update();
};


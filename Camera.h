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

		glm::mat4* getViewMatrixP()		{ return &viewMatrix; }
		GLfloat	   getSensitivity()		{ return sensitivity; }

		void addYaw(GLfloat yaw);
		void addPitch(GLfloat pitch);
		void setRadius(GLfloat radius);

		void mouseControl(GLFWwindow* window, GLfloat xChange, GLfloat yChange, GLfloat& scrollValue, const float& deltaTime);
		void update();

	private:
		glm::mat4  viewMatrix;
		glm::vec3  position, front, right, up, worldUp;
		glm::vec3* target;
		GLfloat yaw = -180.0f, pitch = 0.0f, radius = 30.0f, sensitivity = 150.0f;

};


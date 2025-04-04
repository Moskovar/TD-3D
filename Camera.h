#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <iostream>
#include <map>

using namespace std;

class Camera
{
	public:
		Camera(glm::vec3* target, GLfloat* targetYaw, map<char, bool>* keyPressed);

		//--- Getters ---//
		GLfloat		getYaw()				{ return yaw;							}
		glm::mat4*	getViewMatrixP()		{ return &viewMatrix;					}
		GLfloat		getSensitivity()		{ return sensitivity;					}
		glm::vec3	getUp()					{ return glm::vec3(0.0f, 1.0f, 0.0f);	}

		void addYaw(GLfloat yaw);
		void addPitch(GLfloat pitch);
		void setRadius(GLfloat radius);
		void resetYaw() { this->yaw = *targetYaw; }

		void mouseControl(GLFWwindow* window, GLfloat xChange, GLfloat yChange, GLfloat& scrollValue, const float& deltaTime);
		void update();

	private:
		glm::mat4  viewMatrix;
		glm::vec3  position, front, right;
		glm::vec3* target;
		GLfloat yaw = -90.0f, pitch = 0.0f, radius = 30.0f, sensitivity = 300.0f;
		GLfloat* targetYaw = nullptr;
		map<char, bool>* keyPressed = nullptr;


};


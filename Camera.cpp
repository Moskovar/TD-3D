#include "Camera.h"
#include "uti.hpp"

Camera::Camera(glm::vec3* target, GLfloat* targetYaw, map<char, bool>* keyPressed)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
    position.x = target->x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    position.y = target->y + radius * sin(glm::radians(pitch));
    position.z = target->z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	this->targetYaw  = targetYaw;
	this->keyPressed = keyPressed;

	this->target     = target;
	this->target->y += 3.0f;
    this->front      = glm::normalize(position - *target);
    this->right      = glm::normalize(glm::cross(getUp(), front));

    viewMatrix = glm::lookAt(position, *target, getUp());
}

void Camera::update()
{

    position.x = target->x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    position.y = target->y + radius * sin(glm::radians(pitch));
    position.z = target->z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	//float time = static_cast<float>(glfwGetTime());
	//std::cout << up.x << " ... " << up.y << " ... " << up.z << std::endl;
	//position.x = sin(time) * 20;
	//position.z = cos(time) * 20;

	
    viewMatrix = glm::lookAt(position, *target, getUp());
}

void Camera::addYaw(GLfloat yaw)
{
	//std::cout << "addYaw" << std::endl;
	this->yaw += yaw;

	//std::cout << this->yaw << std::endl;

	if		(this->yaw >  360) this->yaw -= 360;
	else if (this->yaw < -360) this->yaw += 360;
}

void Camera::addPitch(GLfloat pitch)
{
	this->pitch += pitch;

	if		(this->pitch > 89) this->pitch = 89;
	else if (this->pitch < 1 ) this->pitch = 1;
}

void Camera::setRadius(GLfloat radius)
{
	this->radius -= radius;

	if      (this->radius > 30) this->radius = 30;
	else if (this->radius < 0 ) this->radius = 0;
}

void Camera::mouseControl(GLFWwindow* window, GLfloat xChange, GLfloat yChange, GLfloat& scrollValue, const float& deltaTime)
{
	//xChange *= turnSpeed;//USE TURNSPEED OU VELOCITY ???

	if		(xChange > 0) addYaw(deltaTime * sensitivity);
	else if (xChange < 0) addYaw(-deltaTime * sensitivity);

	if		(yChange > 0) addPitch(-deltaTime * sensitivity / 2);//pour clique gauche et droit
	else if (yChange < 0) addPitch(deltaTime * sensitivity / 2);

	if (scrollValue != 0)
	{
		setRadius(scrollValue);
		scrollValue = 0;
	}

	update();
}

#include "Camera.h"
#include "uti.hpp"

Camera::Camera(glm::vec3* target, GLfloat* targetYaw, map<char, bool>* keyPressed)
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->position.x = target->x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	this->position.y = target->y + radius * sin(glm::radians(pitch));
	this->position.z = target->z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	this->target = target;

	this->keyPressed = keyPressed;

	//this->target->y += 3.0f;
    this->front      = glm::normalize(this->position - *target);
    this->right      = glm::normalize(glm::cross(getUp(), front));

    viewMatrix = glm::lookAt(position, *target, getUp());
}

Camera::~Camera()
{
}

void Camera::update()
{

    position.x = target->x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    position.y = target->y + radius * sin(glm::radians(pitch));
    position.z = target->z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	
	this->front = glm::normalize(this->position - *target);
	this->right = glm::normalize(glm::cross(getUp(), front));
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

	if      (this->radius > 100) this->radius = 100;
	else if (this->radius < 0 ) this->radius = 0;
}

void Camera::mouseControl(GLFWwindow* window, GLfloat mouseX, GLfloat mouseY, GLfloat& scrollValue, const float& deltaTime)
{
	//xChange *= turnSpeed;//USE TURNSPEED OU VELOCITY ???

	//if		(mouseX > 0) addYaw(deltaTime * sensitivity);
	//else if (mouseX < 0) addYaw(-deltaTime * sensitivity);

	//if		(mouseY > 0) addPitch(-deltaTime * sensitivity / 2);//pour clique gauche et droit
	//else if (mouseY < 0) addPitch(deltaTime * sensitivity / 2);

	if		(mouseX <= 5)		addYaw(deltaTime * sensitivity); 
	else if (mouseX >= 1915)	addYaw(-deltaTime * sensitivity);

	if (mouseY <= 5)		addPitch(deltaTime * sensitivity / 2);
	else if (mouseY >= 1075)	addPitch(-deltaTime * sensitivity / 2);


	if (scrollValue != 0)
	{
		setRadius(scrollValue);
		scrollValue = 0;
	}

	update();
}

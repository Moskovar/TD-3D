#include "Camera.h"

Camera::Camera(glm::vec3* target)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
    position.x = target->x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    position.y = target->y + radius * sin(glm::radians(pitch));
    position.z = target->z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	this->target   = target;
    this->front    = glm::normalize(position - *target);
    glm::vec3 up   = glm::vec3(0.0f, 1.0f, 0.0f);
	worldUp		   = up;
    this->right    = glm::normalize(glm::cross(up, front));
    this->up       = glm::cross(front, right);

    viewMatrix = glm::lookAt(position, *target, up);
}

void Camera::update()
{
    position.x = target->x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    position.y = target->y + radius * sin(glm::radians(pitch));
    position.z = target->z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    viewMatrix = glm::lookAt(position, *target, up);
}

void Camera::addYaw(GLfloat yaw)
{
	this->yaw += yaw;

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
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))//|| glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) 
	{
		if		(xChange > 0) addYaw( deltaTime * sensitivity);
		else if (xChange < 0) addYaw(-deltaTime * sensitivity);

		if		(yChange > 0) addPitch(-deltaTime * sensitivity / 2);//pour clique gauche et droit
		else if (yChange < 0) addPitch( deltaTime * sensitivity / 2);
	}
	else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
	{
		if		(yChange > 0) addPitch(-deltaTime * sensitivity / 2);//pour clique gauche et droit
		else if (yChange < 0) addPitch( deltaTime * sensitivity / 2);
	}

	if (scrollValue != 0)
	{
		setRadius(scrollValue);
		scrollValue = 0;
	}

	update();
}

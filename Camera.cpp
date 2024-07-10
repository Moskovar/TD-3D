#include "Camera.h"

Camera::Camera(glm::vec3 target)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
    position.x = target.x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    position.y = target.y + radius * sin(glm::radians(pitch));
    position.z = target.z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	this->target   = target;
    this->front    = glm::normalize(position - target);
    glm::vec3 up   = glm::vec3(0.0f, 1.0f, 0.0f);
	worldUp		   = up;
    this->right    = glm::normalize(glm::cross(up, front));
    this->up       = glm::cross(front, right);

    viewMatrix = glm::lookAt(position, target, up);
}

void Camera::update()
{
    position.x = target.x + radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    position.y = target.y + radius * sin(glm::radians(pitch));
	cout << position.y << endl;
    position.z = target.z + radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    viewMatrix = glm::lookAt(position, target, up);
}

void Camera::mouseControl(GLFWwindow* window, GLfloat xChange, GLfloat yChange, GLfloat& scrollValue)
{
	//xChange *= turnSpeed;//USE TURNSPEED OU VELOCITY ???
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) || glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		if (xChange > 0) 
		{
			yaw += 5;
			if (yaw >= 180) yaw = 0;
		}
		else if (xChange < 0)
		{
			yaw -= 5;
			if (yaw <= -180) yaw = 0;
		}

		if (yChange > 0) 
		{
			pitch -= 5;
			if (pitch < 0) pitch = 0;
		}
		else if (yChange < 0) {
			pitch += 5;
			if (pitch > 90) pitch = 90;
		}
	}

	

	if (scrollValue != 0)
	{
		radius -= scrollValue;
		scrollValue = 0;

		if (radius > 30) radius = 30;
		else if (radius < 0) radius = 0;
	}

	update();


	//if (*yOffset > 0) {
	//	if (y_LookAt > y_min_LookAt) {
	//		y_LookAt -= y_LookAt_Ratio;
	//	}
	//	if (position.y > y_min_distance) {
	//		position.y -= y_distance_ratio;
	//	}
	//	if (position.z < z_min_distance)
	//		position.z += z_distance_ratio;

	//	*yOffset = 0.0f;
	//}
	//else if (*yOffset < 0) {//mettre variable max distance

	//	if (y_LookAt < y_max_LookAt) {
	//		y_LookAt += y_LookAt_Ratio;
	//	}
	//	if (position.y < y_max_distance) {
	//		position.y += y_distance_ratio;
	//	}
	//	if (position.z > z_max_distance)
	//		position.z -= z_distance_ratio;
	//	*yOffset = 0.0f;
	//}

	/*yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();*/
}

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 target)
{
	this->position = position;
	this->target   = target;

    this->front    = glm::normalize(position - target);
    glm::vec3 up   = glm::vec3(0.0f, 1.0f, 0.0f);
    this->right    = glm::normalize(glm::cross(up, front));
    this->up       = glm::cross(front, right);
}

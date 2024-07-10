#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

class Camera
{
	public:
		Camera(glm::vec3 position, glm::vec3 target);

		glm::mat4 getViewMatrix() { return glm::lookAt(position, target, up); }

	private:
		glm::vec3 position, front, right, up, target;
};


#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>

#include <map>
#include <string>

#define MAX_HEIGHT	255
#define TURN_SPEED	100		//character turn speed
#define FALL_SPEED	25.0f	//character fall speed
#define JUMP_HIGH	4.0f

struct AABB
{
	glm::vec3 min_point = glm::vec3(0.0f, 0.0f, 0.0f), max_point = glm::vec3(0.0f, 0.0f, 0.0f);
};

struct HeightMapVertex
{
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;
	unsigned int indice = 0;
	glm::vec2 texCoords;
};
#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>

#include <map>
#include <string>

#define TURN_SPEED 100	//character turn speed
#define FALL_SPEED 5.0f	//character fall speed

struct AABB
{
	glm::vec3 min_point = glm::vec3(0.0f, 0.0f, 0.0f), max_point = glm::vec3(0.0f, 0.0f, 0.0f);
};

struct HeightMapVertex
{
	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;
	unsigned int indice = 0;
};

//namespace uti
//{
//
//	//std::map<unsigned short, std::string> keyFrames
//	//{
//	//	{ 0, "RotationX" }
//	//};
//}
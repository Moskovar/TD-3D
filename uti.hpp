#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>

#include <map>
#include <string>

#define MAX_HEIGHT	255
#define TURN_SPEED	100		//character turn speed
#define FALL_SPEED	25.0f	//character fall speed
#define JUMP_HIGH	4.0f

#define TILE_SIZE 32

#define LARGETILE_SIZE			512
#define LARGETILE_ARR_SIZE		16
#define JUNCTION_VERTICES_SIZE	2 * (LARGETILE_ARR_SIZE - 1) //* 2 car chaque jointure prendre les vertices haut d'une tuile et bas de la suivante (donc pas réutilisable d'une jointure à l'autre)
															//Il y a TILE_SIZE - 1 joint à faire sur l'axe z (y du tableau) | y: nombre de Tile, x: nombre de vertex sur une rangée X d'une Tile	
#define CHUNK_ARR_SIZE 2 //2x2

#define MAP_ARR_SIZE 2 //2x2

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
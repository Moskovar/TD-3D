#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>

#include <map>
#include <string>

//--- Shaders ---//
#define MAP_JUNCTIONS_SHADERS				"map_shaders"
#define CHUNKS_JUNCTIONS_SHADERS			"chunks_shaders"
#define LARGETILES_SHADERS					"largetiles_shaders"

#define MAX_HEIGHT	500
#define TURN_SPEED	300		//character turn speed
#define FALL_SPEED	25.0f	//character fall speed
#define JUMP_HIGH	4.0f

#define TILE_SIZE 32

#define LARGETILE_SIZE				512
#define LARGETILE_ARR_SIZE			16

#define CHUNK_ARR_SIZE 2 //2x2

#define MAP_ARR_SIZE 2 //2x2 donc Map = 2x2x2x2 LargesTiles -> MAP_ARR_SIZE² * CHUNK_ARR_SIZE² 

//nombre de side pour les jointures dans LT (par paires)		 //* 2 car chaque jointure prendre les vertices haut d'une tuile et bas de la suivante (donc pas réutilisable d'une jointure à l'autre)
#define LT_JUNCTION_VERTICES_SIZE	2 * (LARGETILE_ARR_SIZE - 1)//Il y a TILE_SIZE - 1 joint à faire sur l'axe z (y du tableau) | y: nombre de Tile, x: nombre de vertex sur une rangée X d'une Tile	
																
//nombre de side pour les jointures dans CK (par paires)
#define CK_JUNCTION_VERTICES_SIZE	2 * (CHUNK_ARR_SIZE - 1)

//nombre de side pour les jointures dans MP (par paires)
#define MP_JUNCTION_VERTICES_SIZE	2 * (MAP_ARR_SIZE - 1)

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

extern std::map <int, std::string>	gl_textures_string;
extern std::map<int, AABB>			obj_hitboxes;

enum HITBOXES
{
	FOUNDATION
};

enum DirectionValue
{
	NONE = 0,

	FORWARD = 1,
	RIGHT = 2,
	BACKWARD = 4,
	LEFT = 8,

	FORWARD_RIGHT = 3,
	FORWARD_BACK = 5,
	FORWARD_LEFT = 9,
	BACK_RIGHT = 6,
	RIGHT_LEFT = 10,
	BACK_LEFT = 12,

	FORWARD_RIGHT_BACK = 7,
	FORWARD_RIGHT_LEFT = 11,
	FORWARD_LEFT_BACK = 13,
	BACK_RIGHT_LEFT = 14,

	ALL = 15
};
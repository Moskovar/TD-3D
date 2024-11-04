#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>

#include <map>
#include <string>
#include <array>
#include <iostream>
#include <mutex>

#define CAM_MAX_DISTANCE 125

//--- Shaders ---//
#define MAP_JUNCTIONS_SHADERS				"map_shaders"
#define CHUNKS_JUNCTIONS_SHADERS			"chunks_shaders"
#define LARGETILES_SHADERS					"largetiles_shaders"

#define MAX_HEIGHT		500
#define TURN_SPEED		300		//character turn speed
#define FALL_SPEED		25.0f	//character fall speed
#define JUMP_HIGH		4.0f
#define WORLD_Y_OFFSET	100.0f

#define TILE_SIZE 32

#define LARGETILE_SIZE				512
#define LARGETILE_ARR_SIZE			16

#define CHUNK_ARR_SIZE 2 //2x2

#define MAP_ARR_SIZE 2 //2x2 donc Map = 2x2x2x2 LargesTiles -> MAP_ARR_SIZE� * CHUNK_ARR_SIZE� 

//nombre de side pour les jointures dans LT (par paires)		 //* 2 car chaque jointure prendre les vertices haut d'une tuile et bas de la suivante (donc pas r�utilisable d'une jointure � l'autre)
#define LT_JUNCTION_VERTICES_SIZE	2 * (LARGETILE_ARR_SIZE - 1)//Il y a TILE_SIZE - 1 joint � faire sur l'axe z (y du tableau) | y: nombre de Tile, x: nombre de vertex sur une rang�e X d'une Tile	
																
//nombre de side pour les jointures dans CK (par paires)
#define CK_JUNCTION_VERTICES_SIZE	2 * (CHUNK_ARR_SIZE - 1)

//nombre de side pour les jointures dans MP (par paires)
#define MP_JUNCTION_VERTICES_SIZE	2 * (MAP_ARR_SIZE - 1)

struct AABB
{
	glm::vec3 min_point = glm::vec3(0.0f, 0.0f, 0.0f), max_point = glm::vec3(0.0f, 0.0f, 0.0f);
};

struct OBB 
{
	glm::vec3 center;  // Position centrale de la hitbox
	glm::mat3 orientation;  // Matrice 3x3 d�finissant les axes locaux (orientations)
	glm::vec3 halfSize;  // Moiti� de la taille sur chaque axe
	glm::vec3 minPoint;
	glm::vec3 maxPoint;

	void updateBounds() 
	{
		glm::vec3 localMin = -halfSize;
		glm::vec3 localMax = halfSize;

		// Calculer les coins transform�s (appliquer l'orientation sur chaque axe)
		glm::vec3 xAxis = orientation[0] * halfSize.x;
		glm::vec3 yAxis = orientation[1] * halfSize.y;
		glm::vec3 zAxis = orientation[2] * halfSize.z;

		// Calculer les 8 coins de l'OBB dans l'espace monde
		std::array<glm::vec3, 8> corners = 
		{
			center - xAxis - yAxis - zAxis,  // Coin inf�rieur arri�re gauche
			center + xAxis - yAxis - zAxis,  // Coin inf�rieur arri�re droit
			center - xAxis + yAxis - zAxis,  // Coin sup�rieur arri�re gauche
			center + xAxis + yAxis - zAxis,  // Coin sup�rieur arri�re droit
			center - xAxis - yAxis + zAxis,  // Coin inf�rieur avant gauche
			center + xAxis - yAxis + zAxis,  // Coin inf�rieur avant droit
			center - xAxis + yAxis + zAxis,  // Coin sup�rieur avant gauche
			center + xAxis + yAxis + zAxis   // Coin sup�rieur avant droit
		};

		// Initialiser minPoint et maxPoint � la premi�re valeur
		minPoint = corners[0];
		maxPoint = corners[0];

		// Trouver les min et max sur chaque axe
		for (const auto& corner : corners) 
		{
			minPoint = glm::min(minPoint, corner);  // Trouve les composantes minimales
			maxPoint = glm::max(maxPoint, corner);  // Trouve les composantes maximales
		}

		//std::cout << "MP: " << maxPoint.x << " : " << maxPoint.y << " : " << maxPoint.z << std::endl;
		//std::cout << "mp: " << minPoint.x << " : " << minPoint.y << " : " << minPoint.z << std::endl;
	}
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

enum HumanTowers
{
	ArcherTower = 1
};

int getCellCenter(GLfloat xOrZ, int cellWidth = 8);

//--- Mutex ---//
extern std::mutex r_entities_mutex;//right player entities mutex
extern std::mutex l_entities_mutex;//left  player entities mutex
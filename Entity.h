#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Element.h"
#include <string>

class Entity : public Element
{
	public:
		Entity() {}
		Entity(short id, glm::vec3 position, const std::string& filePath);

		bool	isMoving()							{ return moving;			}
		uint8_t getDirectionKeys()					{ return directionKeys;		}

		void setMove(bool state)					{ moving = state;			}
		void addY(GLfloat y);
		void directionPressed(uint8_t keyPressed)	{ directionKeys += directionsValue[keyPressed]; updateAnimationID(); }
		void directionReleased(uint8_t keyPressed)	{ directionKeys -= directionsValue[keyPressed]; updateAnimationID(); }

		void calculateHitBox();

	private:
		//bool moving = false;
		

		uint8_t directionKeys;//entier non signé de 0 à 255 -> à mettre dans une autre classe qui gère les entrées claviers ?

		std::map<uint8_t, uint8_t> directionsValue = { {GLFW_KEY_W, 1}, {GLFW_KEY_D, 2}, {GLFW_KEY_S, 4}, {GLFW_KEY_A, 8} };
		
		enum DirectionValue
		{
			NONE				= 0,

			FORWARD				= 1,
			RIGHT				= 2,
			BACKWARD			= 4,
			LEFT				= 8,

			FORWARD_RIGHT		= 3,
			FORWARD_BACK		= 5,
			FORWARD_LEFT		= 9,
			BACK_RIGHT			= 6,
			RIGHT_LEFT			= 10,
			BACK_LEFT			= 12,

			FORWARD_RIGHT_BACK	= 7,
			FORWARD_RIGHT_LEFT	= 11,
			FORWARD_LEFT_BACK	= 13,
			BACK_RIGHT_LEFT		= 14,

			ALL					= 15
		};
		
		void updateAnimationID();
};


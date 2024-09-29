#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Element.h"

class Entity : public Element
{
	public:
		Entity() {}
		Entity(short id, glm::vec3 position, const std::string& filePath);

		//--- Getters ---//
		bool	isJumping()							{ return jumping;			}

		//--- Setters ---//
		void setJumping(bool state)					{ jumping = state;														}
		void directionPressed(uint8_t keyPressed)	{ movingValue += directionsValue[keyPressed]; updateAnimationID();	}
		void directionReleased(uint8_t keyPressed)	{ movingValue -= directionsValue[keyPressed]; updateAnimationID();	}

		void jump(GLfloat deltaTime);

		void calculateHitBox();

	private:
		bool jumping = false;
		GLfloat jumpValue = 0.0f;

		std::map<uint8_t, uint8_t> directionsValue = { {GLFW_KEY_W, 1}, {GLFW_KEY_D, 2}, {GLFW_KEY_S, 4}, {GLFW_KEY_A, 8} };
				
		void updateAnimationID();
};


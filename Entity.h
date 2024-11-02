#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

#include "Element.h"

enum class MovementState
{
	IDLE, MOVING
};

enum class GravityState
{
	GROUNDED, JUMPING, FALLING
};

class Entity : public Element
{
	public:
		Entity() {}
		Entity(short id, glm::vec3 position, const std::string& filePath);

		//--- Getters ---//
		bool	isJumping()							{ return jumping;				}
		bool	isAlive()							{ return alive;					}

		//--- Setters ---//
		void setMovementState(MovementState state)	{ this->movementState = state;	}
		void setGravityState(GravityState state)	{ this->gravityState  = state;	}

		void setJumping(bool state)					{ jumping = state;													}
		void directionPressed(uint8_t keyPressed)	{ movingValue += directionsValue[keyPressed]; updateAnimationID();	}
		void directionReleased(uint8_t keyPressed)	{ movingValue -= directionsValue[keyPressed]; updateAnimationID();	}

		void takeDamages(int damages);
		void jump(GLfloat deltaTime);

		void handleMovement(GLfloat deltaTime);
		void handleGravity(GLfloat deltaTime);

	private:
		int hp = 100;
		bool alive = true, jumping = false;
		GLfloat jumpValue = 0.0f;
		MovementState movementState = MovementState::IDLE;
		GravityState gravityState	= GravityState::GROUNDED;


		std::map<uint8_t, uint8_t> directionsValue = { {GLFW_KEY_W, 1}, {GLFW_KEY_D, 2}, {GLFW_KEY_S, 4}, {GLFW_KEY_A, 8} };
				
		void updateAnimationID();
};


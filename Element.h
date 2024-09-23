#pragma once
#include "Model.h"
#include "uti.hpp"

#define NUM_BONES  100

class Element
{
	public:
		Element() {};
		Element(short id, glm::vec3 position, const std::string& filePath);

		~Element();

		//--- Getters ---//
		Model*		getModel()			{ return model;					}
		glm::vec3	getPosition()		{ return position;				}
		glm::vec3*	getPositionP()		{ return &position;				}
		GLfloat		getY()				{ return position.y;			}
		GLfloat		getYaw()			{ return yaw;					}
		GLfloat*	getPYaw()			{ return &yaw;					}
		AABB&		getRHitbox()		{ return hitBox;				}
		bool		isMoving()			{ return moving;				}
		bool		isFalling()			{ return falling;				}
		glm::mat4	getModelMtx()		{ return modelMatrix;			}
		uint8_t		getDirectionValue() { return directionValue;		}

		glm::vec3	getAnticipateMove(GLfloat deltaTime);
		glm::vec3   getAnticipateFall(GLfloat deltaTime);
		AABB		getAnticipatedMoveHitbox(GLfloat deltaTime);
		AABB		getAnticipatedFallHitbox(GLfloat deltaTime);
		
		
		//--- Setters ---//
		void setAnimationID(uint8_t id)			{ animationID = id;					}
		void setPosition(glm::vec3 position)	{ this->position	= position;		}
		void setYaw(GLfloat yaw);
		void setMove(bool state);
		void setFall(bool state);
		void setModelMtx(glm::mat4 modelMatrix);

		void moveForward(GLfloat z);
		void move(GLfloat deltaTime);
		void moveUp(GLfloat positionY);
		void turn(GLfloat yaw);
		void fall(GLfloat deltaTime);
		void updatePosition();
		
		void render(GLuint& modelLoc, GLuint& bonesTransformsLoc, float& timeSinceStart);

	protected:
		uint8_t id = -1, animationID = 0, directionValue = 0;//entier non signé de 0 à 255 -> à mettre dans une autre classe qui gère les entrées claviers ?

		//--- POSITION ---//
		Model*    model       = nullptr;
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 position	  = glm::vec3(0.0f, 0.0f, 0.0f);
		GLfloat yaw = -90;//à descendre dans entity ? avec render en méthode abstraite ?
		GLfloat moveSpeed = 25;
		bool moving = false, falling = false;

		//--- HITBOX ---//
		AABB hitBox;
		glm::vec3 halfSize = glm::vec3(0.0f, 0.0f, 0.0f);

		void calculateHitBox();
};


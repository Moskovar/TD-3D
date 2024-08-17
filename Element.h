#pragma once
#include "Model.h"

#define NUM_BONES  100

class Element
{
	public:
		Element() {};
		Element(short id, glm::vec3 position, const std::string& filePath);

		~Element();

		//--- Getters ---//
		Model* getModel()			{ return model;					}
		glm::vec3* getPositionP()	{ return &position;				}
		GLfloat* getPYaw()			{ return &yaw;					}
		AABB& getRHitbox()			{ return model->getRHitbox();	}
		
		//--- Setters ---//
		void setAnimationID(uint8_t id) { animationID = id; }

		void move(GLfloat deltaTime);
		void turn(GLfloat yaw);
		void fall(GLfloat deltaTime);
		void updatePosition();
		

		void render(GLuint& modelLoc, GLuint& bonesTransformsLoc, float& animationTime, int idLoc);

	protected:
		uint8_t id = -1, animationID = 0;
		Model*    model       = nullptr;
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 position	  = glm::vec3(0.0f, 0.0f, 0.0f);
		GLfloat yaw = -90;//à descendre dans entity ? avec render en méthode abstraite ?
		GLfloat moveSpeed = 5;
		bool moving = false;
};


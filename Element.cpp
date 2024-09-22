#include "Element.h"

Element::Element(short id, glm::vec3 position, const std::string& filePath)
{
	model		   = new Model(filePath);
	this->id	   = id;

	this->position = position;
	halfSize = (model->getMaxPoint() - model->getMinPoint()) / 2.0f;

	model->translate(modelMatrix, position);
	calculateHitBox();
	if (id == 2)
	{
		hitBox.max_point = model->getMaxPoint();
		hitBox.min_point = model->getMinPoint();
		std::cout << "HB MAXP Y: " << hitBox.max_point.y << std::endl;
 	}
	updatePosition();
}

Element::~Element()
{
	if (model)
	{
		delete model;
		model = nullptr;
	}
	printf("||--- Element is cleared ---||\n");
}

void Element::setYaw(GLfloat yaw)
{
	this->yaw = yaw;	
	modelMatrix = glm::rotate(modelMatrix, glm::radians(this->yaw - yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	updatePosition();
}

void Element::setMove(bool state)
{
	if (!isFalling()) moving = state;
}

void Element::setFall(bool state)
{
	falling = state;
	if (!falling) moving = false;
}

void Element::setModelMtx(glm::mat4 modelMatrix)
{
	this->modelMatrix = modelMatrix;
	updatePosition();
}

/// <summary>
/// Récupère la position de l'élément si le mouvement qu'il s'apprête à faire s'effectue
/// </summary>
/// <param name="deltaTime"></param>
/// <returns> retourne la position anticipée</returns>
glm::vec3 Element::anticipateMove(GLfloat deltaTime)
{
	glm::mat4 mtx = modelMatrix;
	mtx = glm::translate(mtx, glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
	return glm::vec3(mtx[3].x, mtx[3].y, mtx[3].z);
}

AABB Element::getAnticipatedHitbox(GLfloat deltaTime)
{
	glm::vec3 anticipatedPosition = anticipateMove(deltaTime);
	AABB anticipatedHitbox;
	anticipatedHitbox.max_point = anticipatedPosition + halfSize;
	anticipatedHitbox.min_point = anticipatedPosition - halfSize;

	return anticipatedHitbox;
}

void Element::move(GLfloat deltaTime)
{
	model->translate(modelMatrix, glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
	updatePosition();
}

void Element::moveUp(GLfloat positionY)
{
	model->translate(modelMatrix, glm::vec3(0.0f, positionY - position.y, 0.0f));
	updatePosition();
}

void Element::turn(GLfloat yaw)//et le deltatime ??
{
	this->yaw += yaw;
	if		(this->yaw >  360) this->yaw -= 360;
	else if (this->yaw < -360) this->yaw += 360;

	modelMatrix = glm::rotate(modelMatrix, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	updatePosition();
	//std::cout << this->yaw << std::endl;
}

void Element::fall(GLfloat deltaTime)
{
	model->translate(modelMatrix, glm::vec3(0.0f, -FALL_SPEED * deltaTime, 0.0f));
	updatePosition();
}

void Element::calculateHitBox()
{
	hitBox.max_point = position + halfSize;
	hitBox.min_point = position - halfSize;

	//hitBox.max_point = position + model->getMaxPoint();
	//hitBox.min_point = position - model->getMinPoint();
}

void Element::updatePosition()
{
	position.x = modelMatrix[3].x;
	position.y = modelMatrix[3].y;
	position.z = modelMatrix[3].z;

	calculateHitBox();
}

void Element::render(GLuint& modelLoc, GLuint& bonesTransformsLoc, float& timeSinceStart)
{
	float ticksPerSecond = 0, duration = 0 , animationTime = 0;
	glm::mat4 bonesTransform[NUM_BONES] = {};
	if (model->getAnimations().size() > 0)
	{
		//printf("AnimationID: %d\n", animationID);
		//printf("AnimationID: %d ... Duration: %f\n", animationID, model->getAnimation(animationID)->getDuration());
		ticksPerSecond = model->getAnimation(animationID)->getTicksPerSecond();
		duration = model->getAnimation(animationID)->getDuration() / ticksPerSecond;
		animationTime = fmod(timeSinceStart * ticksPerSecond, duration);

		glm::mat4 rootMatrix = glm::mat4(1.0f);
		model->getAnimation(animationID)->getRootBone()->interpolateTransform(animationID, animationTime, bonesTransform, rootMatrix);

		glUniformMatrix4fv(bonesTransformsLoc, NUM_BONES, GL_FALSE, &bonesTransform[0][0][0]);
	}

	//glUniform1i(idLoc, id);//debug - affichage couleurs différentes

	//Passer les matrices de vue et de projection aux shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	model->renderModel();
}


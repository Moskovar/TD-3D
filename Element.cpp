#include "Element.h"

Element::Element(short id, glm::vec3 position, const std::string& filePath)
{
	model		   = new Model(filePath);
	this->id	   = id;

	this->position = position;

	halfSize = (model->getMaxPoint() - model->getMinPoint()) * 0.5f;

	model->translate(modelMatrix, position);
	
	updatePosition();
}

Element::Element(short id, glm::mat4 modelMtx, const std::string& filePath)
{
	model = new Model(filePath);
	this->id = id;

	this->modelMatrix = glm::mat4(modelMtx);

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
	if (!falling && movingValue == 0) moving = false;
}

void Element::setModelMtx(glm::mat4 modelMatrix)
{
	this->modelMatrix = modelMatrix;
	updatePosition();
}

/// <summary>
/// R�cup�re la position de l'�l�ment si le mouvement qu'il s'appr�te � faire s'effectue
/// </summary>
/// <param name="deltaTime"></param>
/// <returns> retourne la position anticip�e</returns>
glm::mat4 Element::getAnticipatedMove(GLfloat deltaTime)
{
	glm::mat4 mtx = modelMatrix;
	mtx = glm::translate(mtx, glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
	return mtx;
}

glm::mat4 Element::getAnticipatedFall(GLfloat deltaTime)
{
	glm::mat4 mtx = modelMatrix;
	mtx = glm::translate(mtx, glm::vec3(0.0f, -FALL_SPEED * deltaTime, 0.0f));
	return mtx;
}

OBB Element::getAnticipatedMoveHitbox(GLfloat deltaTime)
{
    glm::mat4 anticipatedMtx = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, moveSpeed * deltaTime));
    
    OBB anticipatedHitbox;
    anticipatedHitbox.center = glm::vec3(anticipatedMtx[3].x, anticipatedMtx[3].y, anticipatedMtx[3].z);

    // Assurer que l'orientation est correcte
    anticipatedHitbox.orientation = glm::mat3(anticipatedMtx);

	//std::cout << "HITBOX: " << std::endl;

	//std::cout << hitbox.orientation[0].x << " : " << hitbox.orientation[0].y << " : " << hitbox.orientation[0].z << std::endl;
	//std::cout << hitbox.orientation[1].x << " : " << hitbox.orientation[1].y << " : " << hitbox.orientation[1].z << std::endl;
	//std::cout << hitbox.orientation[2].x << " : " << hitbox.orientation[2].y << " : " << hitbox.orientation[2].z << "\n" << std::endl;

	//std::cout << hitbox.center.x << " ... " << hitbox.center.y << " ... " << hitbox.center.z << "\n\n\n" << std::endl;

	//std::cout << "ANTICIPATEDHITBOX: " << std::endl;

	//std::cout << anticipatedHitbox.orientation[0].x << " : " << anticipatedHitbox.orientation[0].y << " : " << anticipatedHitbox.orientation[0].z << std::endl;
	//std::cout << anticipatedHitbox.orientation[1].x << " : " << anticipatedHitbox.orientation[1].y << " : " << anticipatedHitbox.orientation[1].z << std::endl;
	//std::cout << anticipatedHitbox.orientation[2].x << " : " << anticipatedHitbox.orientation[2].y << " : " << anticipatedHitbox.orientation[2].z << "\n" << std::endl;

	//std::cout << anticipatedHitbox.center.x << " ... " << anticipatedHitbox.center.y << " ... " << anticipatedHitbox.center.z << "\n\n\n" << std::endl;

	glm::vec3 scale = glm::vec3(
		glm::length(this->modelMatrix[0]),
		glm::length(this->modelMatrix[1]),
		glm::length(this->modelMatrix[2])
	);

    // Recalculer halfSize en tenant compte des transformations
    anticipatedHitbox.halfSize = (model->getMaxPoint() - model->getMinPoint()) * 0.5f * scale;

	anticipatedHitbox.updateBounds();

    return anticipatedHitbox;
}

OBB Element::getAnticipatedFallHitbox(GLfloat deltaTime)
{
	glm::mat4 anticipatedMtx = getAnticipatedFall(deltaTime);
	OBB anticipatedHitbox;
	anticipatedHitbox.center = glm::vec3(anticipatedMtx[3].x, anticipatedMtx[3].y, anticipatedMtx[3].z);
	anticipatedHitbox.orientation = glm::mat3(anticipatedMtx);
	anticipatedHitbox.halfSize		= (model->getMaxPoint() - model->getMinPoint()) * 0.5f;//useless car bouge pas ?


	return anticipatedHitbox;
}

void Element::moveForward(GLfloat z)
{
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, z));
	updatePosition();
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
	// Mettre � jour le centre de l'OBB
	hitbox.center = this->position;

	// Extraire l'orientation depuis la matrice de mod�le
	hitbox.orientation = glm::mat3(this->modelMatrix);

	// Calculer l'�chelle � partir des colonnes de la matrice de mod�le
	glm::vec3 scale(
		glm::length(this->modelMatrix[0]),  // �chelle sur l'axe X
		glm::length(this->modelMatrix[1]),  // �chelle sur l'axe Y
		glm::length(this->modelMatrix[2])   // �chelle sur l'axe Z
	);

	// Calculer halfSize en prenant en compte l'�chelle
	hitbox.halfSize = (model->getMaxPoint() - model->getMinPoint()) * 0.5f * scale;

	//std::cout << hitbox.orientation[0].x << " : " << hitbox.orientation[0].y << " : " << hitbox.orientation[0].z << std::endl;
	//std::cout << hitbox.orientation[1].x << " : " << hitbox.orientation[1].y << " : " << hitbox.orientation[1].z << std::endl;
	//std::cout << hitbox.orientation[2].x << " : " << hitbox.orientation[2].y << " : " << hitbox.orientation[2].z << std::endl;

	hitbox.updateBounds();
}


void Element::updatePosition()
{
	this->position.x = this->modelMatrix[3].x;
	this->position.y = this->modelMatrix[3].y;
	this->position.z = this->modelMatrix[3].z;

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

	//glUniform1i(idLoc, id);//debug - affichage couleurs diff�rentes

	//Passer les matrices de vue et de projection aux shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	model->renderModel();
}


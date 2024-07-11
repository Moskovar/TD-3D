#include "Element.h"

Element::Element(short id, glm::vec3 position, const std::string& filePath)
{
	this->position = position;
	model		   = new Model(filePath);
	this->id	   = id;
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

void Element::render(GLuint& modelLoc, GLuint& bonesTransformsLoc, float& deltaTime, int idLoc)
{
	float ticksPerSecond = 0, duration = 0 , animationTime = 0;
	glm::mat4 bonesTransform[NUM_BONES] = {};
	if (model->getAnimations().size() > 0)
	{
		ticksPerSecond = model->getAnimation(1)->getTicksPerSecond();
		duration = model->getAnimation(1)->getDuration() / ticksPerSecond;
		animationTime = fmod(deltaTime * ticksPerSecond, duration);

		glm::mat4 mtx = glm::mat4(1.0f);
		model->getAnimation(1)->getRootBone()->interpolateTransform(animationTime, bonesTransform, mtx);

		glUniformMatrix4fv(bonesTransformsLoc, NUM_BONES, GL_FALSE, &bonesTransform[0][0][0]);
	}

	//if(id == 0) std::cout << position.z << std::endl;
	glUniform1i(idLoc, id);
	modelMatrix = glm::translate(glm::mat4(1.0f), position);


	//Passer les matrices de vue et de projection aux shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	model->renderModel();
}


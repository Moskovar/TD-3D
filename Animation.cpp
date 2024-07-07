#include "Animation.h"

Animation::Animation(std::string name, float duration, float ticksPerSecond, Bone* rootBone)
{
	this->name			 = this->name;
	this->duration		 = duration;
	this->ticksPerSecond = ticksPerSecond;
	this->rootBone		 = rootBone;

	printf("Duration: %f, Ticks: %f\n", duration, ticksPerSecond);
}

void Animation::animate(double animationTime)
{
	//printf("Animation name: %s\n", animation.name.c_str());
	glm::mat4 parentTransform(1.0f);
	rootBone->applyTransformations(rootBone->interpolateTransform(animationTime), parentTransform, animationTime);

	//if(bones["Bone"])
	//	bones["Bone"]->applyTransformations(bones["Bone"]->interpolateTransform(animationTime, animation));
}

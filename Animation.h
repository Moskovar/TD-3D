#pragma once
#include "Bone.h"
#include <map>
#include <vector>

class Animation
{
	public:
		Animation() {}
		Animation(std::string name, float duration, float ticksPerSecond, Bone* rootBone);
		float getDuration()		  { return this->duration; }
		float getTicksPerSecond() { return ticksPerSecond; }

		void animate(double animationTime);

	private:
		Bone* rootBone   = nullptr;
		std::string name = "";
		float duration   = 0, ticksPerSecond = 0;
};


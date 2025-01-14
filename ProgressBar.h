#pragma once
#include "UIElement.h"
#include <string>

class ProgressBar : public UIElement
{
	public:
		ProgressBar() {}
		ProgressBar(int* x, int* y, float progress, ImVec4 color, std::string overlay = "");

		float       progress	= 0;
		std::string overlay		= "";
		ImVec4		color		= ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	private:

};
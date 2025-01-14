#include "ProgressBar.h"

ProgressBar::ProgressBar(int* x, int* y, float progress, ImVec4 color, std::string overlay) : UIElement(x, y)
{
	this->progress = progress;
	this->color = color;
	this->overlay = overlay;
}
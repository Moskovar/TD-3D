#include "UIElement.h"

UIElement::UIElement(int* x, int* y)
{
	this->x = x;
	this->y = y;
}

UIElement::UIElement(int x, int y)
{
	this->x = new int(x);
	this->y = new int(y);
}

UIElement::~UIElement()
{
	if (x)
	{
		delete x;
		x = nullptr;
	}

	if (y)
	{
		delete y;
		y = nullptr;
	}
}

#pragma once
#include "imgui.h"

class UIElement
{
	public:
		UIElement() {}
		UIElement(int* x, int* y);
		UIElement(int  x, int  y);
		~UIElement();

		int getX() { return *x; }
		int getY() { return *y; }

		int* x = nullptr, *y = nullptr;

	protected:
};
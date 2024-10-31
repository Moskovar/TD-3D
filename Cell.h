#pragma once
#include <GL/glew.h>
#include "Tower.h"

class Cell
{
	public:
		Cell(GLfloat x, GLfloat z);

		GLfloat getX()		{ return x;								}
		GLfloat getZ()		{ return z;								}
		bool isBuildable()	{ return buildable && !tower;			}
		
		void setTower(Tower* t) { tower = t; }

	private:
		GLfloat x, z;
		bool	buildable = true;
		Tower*	tower = nullptr;
};


#include "uti.hpp"

std::map <int, std::string> gl_textures_string =
{
	{ GL_TEXTURE0, "texture0" },
	{ GL_TEXTURE1, "texture1" },
	{ GL_TEXTURE2, "texture2" },
	{ GL_TEXTURE3, "texture3" }
};

std::map<int, AABB> obj_hitboxes =
{
	{}
};

int getCellCenter(GLfloat xOrZ, int cellWidth)
{
	return (int)xOrZ / 8 * 8 + 4;
}

//--- Mutex ---//
std::mutex r_entities_mutex;//right player entities mutex
std::mutex l_entities_mutex;//left  player entities mutex
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
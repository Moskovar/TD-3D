#include "uti.hpp"

std::map <int, std::string> gl_textures_string =
{
	{ GL_TEXTURE0, "texture0" },
	{ GL_TEXTURE1, "texture1" },
	{ GL_TEXTURE2, "texture2" },
	{ GL_TEXTURE3, "texture3" }
};

std::map<int, AABB> obj_hitboxes =//??
{
	{}
};

std::map<int, std::string> str_heros =
{
	{ Heros::TitusMage, "TitusMage" }
};

std::map<int, std::string> str_sounds =
{
	{ SoundsID::OOM, "OOM" },
	{ SoundsID::CD,  "CD"  }
};

std::map<int, short> spellsCost =
{
	{ Spells::FireBall, 30 }
};

std::map<std::string, int> animationIDs =
{
	{ "Armature|Run"	, 1 },
	{ "Armature|Attack"	, 2}
};

int getCellCenter(GLfloat xOrZ, int cellWidth)
{
	return (int)xOrZ / 8 * 8 + 4;
}

bool isPointInsideOBB(const glm::vec3& point, const OBB& obb)
{
	// Convertir le point dans le repère local de l'OBB
	glm::vec3 localPoint = glm::transpose(obb.orientation) * (point - obb.center);

	// Vérifier si le point local est dans les limites de l'OBB
	return (std::abs(localPoint.x) <= obb.halfSize.x &&
			std::abs(localPoint.y) <= obb.halfSize.y &&
			std::abs(localPoint.z) <= obb.halfSize.z);
}

int getNow()
{
	auto now = std::chrono::system_clock::now();
	auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	return timestamp;
}

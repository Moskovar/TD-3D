#pragma once
#include <GLM/glm.hpp>             // Pour les types de base (vec3, mat4, etc.)
#include <GLM/gtc/quaternion.hpp> //  Pour les quaternions (quat)
#include <glm/gtx/transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <string>
#include <tuple>

#include "Mesh.h"

struct KeyFrame 
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
	//double time;
};

struct Animation
{
	std::string name;
	double duration;
	double ticksPerSecond;

	bool operator==(const Animation& other) const
	{
		return name == other.name && duration == other.duration;
	}

	// Surcharge de l'opérateur < pour comparer deux MyStruct
	bool operator<(const Animation& other) const {
		if (duration != other.duration) {
			return duration < other.duration;
		}
		return name < other.name;
	}
};

class Bone
{
	public:
		Bone() {};
		Bone(std::vector<std::tuple<float, Vertex*>> vertices);
		~Bone();

		auto getVertices() { return vertices; }
		auto getAnimations() { return animations; }
		void setKeyFrames(Animation animation, std::map<double, KeyFrame> keyFrames) { this->animations[animation] = keyFrames; }
		void addChildren(Bone* bone) { children.push_back(bone); }
		std::vector<Bone*> getChildren() { return children; }
        // Fonction pour interpoler les transformations d'une animation
		glm::mat4 interpolateTransform(double animationTime, Animation animation);
		void applyTransformations(glm::mat4 transformations);
		
	private:
		std::vector<std::tuple<float, Vertex*>> vertices;//poids vertex
		std::map<Animation, std::map<double, KeyFrame>> animations;
		std::vector<Bone*> children;

		// Fonction pour interpoler entre deux valeurs
		template <typename T>
		T interpolate(const T& start, const T& end, float factor);
};

template<typename T>
inline T Bone::interpolate(const T& start, const T& end, float factor) { return start + factor * (end - start); }


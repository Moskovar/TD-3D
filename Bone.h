#pragma once
#include <GLM/glm.hpp>             // Pour les types de base (vec3, mat4, etc.)
#include <GLM/gtc/quaternion.hpp> //  Pour les quaternions (quat)
#define GLM_ENABLE_EXPERIMENTAL
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

class Bone
{
	public:
		Bone() {};
		Bone(int id, std::string name, std::vector<std::tuple<float, Vertex>> vertices);
		~Bone();

		auto getVertices() { return vertices; }
		auto getAnimations() { return keyFrames; }
		std::string getName() { return name; }
		void setKeyFrames(std::map<double, KeyFrame> keyFrames) { this->keyFrames = keyFrames; }
		void addChildren(Bone* bone) { children.push_back(bone); }
		std::vector<Bone*> getChildren() { return children; }
        // Fonction pour interpoler les transformations d'une animation
		void interpolateTransform(double animationTime, glm::mat4* bonesTransform, glm::mat4& parentTransforms);
		void applyTransformations(glm::mat4 localTransform, glm::mat4 parentTransform, double animationTime);
		
	private:
		int id = 0;
		std::string name;
		std::vector<std::tuple<float, Vertex>> vertices;//poids vertex
		std::map<double, KeyFrame> keyFrames;
		std::vector<Bone*> children;

		// Fonction pour interpoler entre deux valeurs
		glm::vec3 interpolate(const glm::vec3& start, const glm::vec3& end, float factor) { return start + factor * (end - start); }
}; 


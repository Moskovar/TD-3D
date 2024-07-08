#pragma once
#include <vector>
#include <map>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <iostream>
#include <chrono>

#include "Bone.h"
#include "Mesh.h"
#include "Animation.h"
//#include "Texture.h"

#define ANIMATION_SPEED_RATE 10.0
#define ANIMATION_ID		 0

struct Node
{
	std::string name;
	std::vector<Node*> children;
};

class Model
{
	public:
		Model() {};
		Model(const std::string& filePath);

		void loadModel(const std::string& fileName);
		void renderModel();
		void clearModel();
		//void animate(double animationTime);

		const std::map<std::string, Bone*> getBones() { return bones; }
		Bone* getBone(std::string boneName) { return bones[boneName]; }
		const auto getAnimations() { return animations; }
		Animation* getAnimation(unsigned short id) { return (animations.size() > 0) ? & animations[id] : nullptr; }

		~Model();

	private:
		void loadNode(aiNode* node, const aiScene *scene, Node* parentNode = nullptr);
		void loadMesh(aiMesh* mesh, const aiScene *scene, std::vector<Mesh*>& meshes);
		//void loadMaterials(const aiScene *scene);

		std::vector<Mesh*> meshList;
		//std::vector<Texture*> textureList;
		std::vector<unsigned int> meshToTex;

		std::map<std::string, Bone*> bones;//on utilise le nom du node pour le retrouver dans le map
		std::vector<Node*> nodes;

		//--- Animations ---//
		std::map<unsigned short, Animation> animations;
};


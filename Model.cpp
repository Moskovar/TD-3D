#include "Model.h"



Model::Model(const std::string& filePath)
{
	loadModel(filePath);
	//printf("Model has %d nodes\n", nodes.size());
	for (Node* node : nodes)
	{
		if (bones[node->name])
		{
			for (Node* childNode : node->children)
			{
				if (bones[childNode->name]);
					bones[node->name]->addChildren(bones[childNode->name]);
			}
		}
	}

	//printf("ANIMATIONS SIZE: %d", bones["Bone"]->getAnimations().size());

	//printf("Model has %d bones\n", bones.size());
	//for (auto it = bones.begin(); it != bones.end(); ++it)
	//{
	//	if (it->second)
	//		printf("Bone: %s has %d children\n", it->first.c_str(), it->second->getChildren().size());
	//	else
	//		printf("%s is not set!\n", it->first.c_str());
	//}

	//for (auto bone : bones)
	//{
	//	if (bone.second)
	//	{
	//		printf("Bone: %s has %d animations ->\n", bone.first.c_str(), bone.second->getAnimations().size());
	//		for (auto animation : bone.second->getAnimations())
	//			printf("Animation: %s duration is %f and has %d keyFrames\n", animation.first.name.c_str(), animation.first.duration, animation.second.size());
	//	}
	//}
	//printf("Size::: %d\n", bones["Bone"]->getAnimations().begin()->second.size());
	const auto animations = bones["Bone"]->getAnimations();
	for (const auto keyFrames : animations.begin()->second)
	{
		printf("Time: %f ... \n", keyFrames.first);
	}
	//
	//for (auto it = animations.begin(); it != animations.end(); ++it)
	//{

	//}
}

void Model::loadModel(const std::string& fileName)
{
	clearModel();

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene) 
	{
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	loadNode(scene->mRootNode, scene);

	scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || !scene->mRootNode) {
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
	}

	printf("\n\n\n");

	// Vérifiez si le modèle contient des animations
	if (scene->mNumAnimations > 0) 
	{
		const aiAnimation* animation = scene->mAnimations[2]; // Récupère la première animation
		Animation str_animation = { std::string(animation->mName.C_Str()), animation->mDuration, animation->mTicksPerSecond };
		//printf("Nombre d'animation pour: %s -> %d\n", fileName.c_str(), scene->mNumAnimations);
		// Vous pouvez maintenant travailler avec l'animation
		// Variables pour gérer le temps
		//printf("Durée d'animation: %f\nNombre de tick par seconde: %f\n", duration, ticksPerSecond);
		//printf("Animation name: %s\n", animation->mName.C_Str());

		for (unsigned int i = 0; i < animation->mNumChannels; i++)//on parcourt os de l'animation
		{
			const aiNodeAnim* bone = animation->mChannels[i];
			std::map<double, KeyFrame> keyFrames;
			double ticksPerSecond = animation->mTicksPerSecond / 4.0;

			if (bones[std::string(bone->mNodeName.C_Str())]) printf("bone name: %s ... Vertices: %d\n", bone->mNodeName.C_Str(), bones[std::string(bone->mNodeName.C_Str())]->getVertices().size());
			else printf("Bone %s is not set\n", bone->mNodeName.C_Str());

			for (unsigned int j = 0; j < bone->mNumPositionKeys; j++) 
			{
				keyFrames[bone->mPositionKeys[j].mTime / ticksPerSecond].position.x = bone->mPositionKeys[j].mValue.x;
				keyFrames[bone->mPositionKeys[j].mTime / ticksPerSecond].position.y = bone->mPositionKeys[j].mValue.y;
				keyFrames[bone->mPositionKeys[j].mTime / ticksPerSecond].position.z = bone->mPositionKeys[j].mValue.z;
				printf("Position:  x: %f ... y: %f ... z: %f\n", keyFrames[bone->mPositionKeys[j].mTime / ticksPerSecond].position.x, keyFrames[bone->mPositionKeys[j].mTime / ticksPerSecond].position.y, keyFrames[bone->mPositionKeys[j].mTime / ticksPerSecond].position.z);
				//printf("Time: pos : %f\n", bone->mPositionKeys[j].mTime);
			}

			//printf("\n");

			for (unsigned int j = 0; j < bone->mNumRotationKeys; j++)
			{
				keyFrames[bone->mRotationKeys[j].mTime / ticksPerSecond].rotation.w = bone->mRotationKeys[j].mValue.w;
				keyFrames[bone->mRotationKeys[j].mTime / ticksPerSecond].rotation.x = bone->mRotationKeys[j].mValue.x;
				keyFrames[bone->mRotationKeys[j].mTime / ticksPerSecond].rotation.y = bone->mRotationKeys[j].mValue.y;
				keyFrames[bone->mRotationKeys[j].mTime / ticksPerSecond].rotation.z = bone->mRotationKeys[j].mValue.z;
				printf("Rotation:  w: %f ... x: %f ... y: %f ... z: %f\n", bone->mRotationKeys[j].mValue.w, bone->mRotationKeys[j].mValue.x, bone->mRotationKeys[j].mValue.y, bone->mRotationKeys[j].mValue.z);
				//printf(" ... rot : %f\n", bone->mRotationKeys[j].mTime);
			}

			//printf("\n");
			
			for (unsigned int j = 0; j < bone->mNumScalingKeys; j++)
			{
				
				keyFrames[bone->mScalingKeys[j].mTime / ticksPerSecond].scale.x = bone->mScalingKeys[j].mValue.x;
				keyFrames[bone->mScalingKeys[j].mTime / ticksPerSecond].scale.y = bone->mScalingKeys[j].mValue.y;
				keyFrames[bone->mScalingKeys[j].mTime / ticksPerSecond].scale.z = bone->mScalingKeys[j].mValue.z;
				printf("Scale: x: %f ... y: %f ... z: %f\n", bone->mScalingKeys[j].mValue.x, bone->mScalingKeys[j].mValue.y, bone->mScalingKeys[j].mValue.z);
				//printf(" ... scal : %f\n" , bone->mScalingKeys[j].mTime);
			}

			//printf("\n\n\n");
			if (bones[std::string(bone->mNodeName.C_Str())]) bones[std::string(bone->mNodeName.C_Str())]->setKeyFrames(str_animation, keyFrames);
			//else printf("%s is not set \n", bone->mNodeName.C_Str());
		}
	}
	else printf("Pas d'animation pour: %s\n", fileName.c_str());

	//for (auto it = nodes.begin(); it != nodes.end(); it++) printf("%s\n", it->first.c_str());//if(it->second) printf("Node name : % s\n", it->second->getName().c_str());

	//printf("\nNombre de noeuds total: %d", nodes.size());

	//loadMaterials(scene);
}

void Model::renderModel(const GLuint& modelLoc, glm::mat4 model)
{
	//std::vector<Vertex*> vertices = bones["Os.011"].getVertices()
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		//if (materialIndex < textureList.size() && textureList[materialIndex])
		//{
		//	textureList[materialIndex]->UseTexture();
		//}

		meshList[i]->RenderMesh(modelLoc, model);
	}
}

void Model::clearModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	//for (size_t i = 0; i < textureList.size(); i++)
	//{
	//	if (textureList[i])
	//	{
	//		delete textureList[i];
	//		textureList[i] = nullptr;
	//	}
	//}
}

void Model::animate(double animationTime, Animation animation)
{
	if(bones["Bone"])
		bones["Bone"]->applyTransformations(bones["Bone"]->interpolateTransform(animationTime, animation));

	for (Mesh* mesh : meshList)
	{
		mesh->updateMesh();
	}
}

Model::~Model()
{
	for (auto it = bones.begin(); it != bones.end(); ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
}

void Model::loadNode(aiNode* node, const aiScene* scene, Node* parentNode)
{
	//if(node->mNumChildren > 0) printf("Nom du noeud: %s ... nombre de meshes: %d\n", node->mName.C_Str(), node->mNumMeshes);

	std::vector<Mesh*> meshes;

	Node* currentNode = new Node{ std::string(node->mName.C_Str()) };
	nodes.push_back(currentNode);
	//printf("Node: %s\n", currentNode->name.c_str());

	if (parentNode) parentNode->children.push_back(currentNode);//Si le node actuel a un parent, on lui ajout le node actuel comme enfant

	for (size_t i = 0; i < node->mNumMeshes; i++) 
	{
		loadMesh(scene->mMeshes[node->mMeshes[i]], scene, meshes);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{ 
		loadNode(node->mChildren[i], scene, currentNode);
	}

	//bones[std::string(node->mName.C_Str())] = Bone(std::string(node->mName.C_Str()), meshes);
}

void Model::loadMesh(aiMesh* mesh, const aiScene* scene, std::vector<Mesh*>& meshes)
{
	std::vector<Vertex*>	  v_vertices;
	std::vector<unsigned int> v_indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex* vertex = new Vertex();
		vertex->id = i;

		vertex->position.x = mesh->mVertices[i].x;
		vertex->position.y = mesh->mVertices[i].y;
		vertex->position.z = mesh->mVertices[i].z;

		// Récupérer les normales
		if (mesh->HasNormals())
		{
			vertex->normal.x = mesh->mNormals[i].x;
			vertex->normal.y = mesh->mNormals[i].y;
			vertex->normal.z = mesh->mNormals[i].z;
		}

		// Récupérer les coordonnées de texture
		if (mesh->mTextureCoords[0])
		{
			vertex->texCoords.x = mesh->mTextureCoords[0][i].x;
			vertex->texCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else vertex->texCoords = glm::vec2(0.0f, 0.0f);

		v_vertices.push_back(vertex);
	}

	if (mesh->HasBones()) 
	{//useless car si 0 on rentre pas dans tableau ?!
		for (size_t i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* bone = mesh->mBones[i];
			printf("Bone: %s has %d vertices\n", bone->mName.C_Str(), bone->mNumWeights);
			std::vector<std::tuple<float, Vertex*>> v_vertices2;//enlever l'autre vecteur pointeur?
			for (size_t j = 0; j < bone->mNumWeights; j++)
			{
				for (Vertex* vertex : v_vertices)//on cherche le vertex correspondant dans le vecteur qu'on vient de remplir et quand on trouve on le met dans le vecteur pour l'os
					if (vertex->id == bone->mWeights[j].mVertexId)
					{
						//vertex->weight = bone->mWeights[j].mWeight;
						v_vertices2.push_back(std::make_tuple(bone->mWeights[j].mWeight, vertex));
					}
			}
			bones[std::string(bone->mName.C_Str())] = new Bone(v_vertices2);
			//printf("Bone: %s has %d vertices\n", bone->mName.C_Str(), bones[std::string(bone->mName.C_Str())].getVertices().size());
		}
	}

	//for (Vertex* vertex : bones["Os.005"]->getVertices())
	//{
	//	vertex->position.x += 50;
	//}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			v_indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh(v_vertices, v_indices);
	//newMesh->CreateMesh();
	meshList.push_back(newMesh);
	meshes.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}

/*void Model::loadMaterials(const aiScene* scene)
{
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE));
		{
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("Textures/") + filename;

				textureList[i] = new Texture(texPath.c_str());

				if (!textureList[i]->LoadTexture())
				{
					printf("Failed to load texture at: %s", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}
		if (!textureList[i])
		{
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTextureA();
		}
	}
}*/

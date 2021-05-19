#pragma once
#include "utils/Utils.h"
#include "Mesh.h"
#include "Joint.h"
#include "AnimatedMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <SOIL2/SOIL2.h>
#include "Animation.h"
#include "Animator.h"
class Animator;
class AnimatedModel : public Actor
{
private:
	//skin
	std::vector<AnimatedMesh> meshes;
	std::vector<AnimatedTexture>textures_loaded;
	string directory;

	//skeleton
	Joint rootJoint;
	int jointCount = 0;

	aiMatrix4x4 m_globalTransform;

	Animator* animator;
	std::vector<Animation> animations;
	
	Animation loadAnimation(aiAnimation* animation);

	void loadModel(string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	AnimatedMesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Joint> loadBones(int meshIndex, const aiMesh* mesh);
	bool findRootBone(aiNode* node, AnimatedMesh* mesh);
	bool findNameinJoints(string name, std::vector<Joint>* joints);
	void createBoneHierarchy(const aiScene* scene);

	std::vector<AnimatedVertex> loadJointWeightsToVertices(aiMesh* mesh, std::vector<AnimatedVertex> vertices);


	void recursiveBoneHierarchy(Joint* parentJoint, const aiScene* scene);

	aiNode* findNodeByName(string name, aiNode* rootNode);

	Joint getJointbyName(string name, std::vector<Joint>* joints);

	void addJointsToArray(Joint headJoint, std::vector<glm::mat4>* jointMatrices);
	glm::mat4 convertToglm(aiMatrix4x4 aiMat);

	void testPrintBoneNames();

	Animation getAnimation(string name);
public:
	AnimatedModel();
	AnimatedModel(string const& path, glm::vec3 position, glm::vec3 scale);

	std::vector<glm::mat4> getJointTransforms();

	void setUniforms(AdvancedShader* shader);
	std::vector<AnimatedTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	unsigned int TextureFromFile(const char* path, const string& directory);

	Joint* getRootJoint() {
		return &rootJoint;
	}
	void setRotation(glm::quat quat) {
		this->transform.setRotation(quat);
	}

	void draw(AdvancedShader* shader);

	void doAnimation(Animation::ANIMATION anim);
	void update(float dt);
};

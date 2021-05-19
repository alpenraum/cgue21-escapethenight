#include "AnimatedModel.h"
#include <assimp\postprocess.h>
#include "JointAnimation.h"

Animation AnimatedModel::loadAnimation(aiAnimation* animation)
{
	float length = animation->mDuration / animation->mTicksPerSecond;

	int numberKeyFrames = 0;
	std::vector<JointAnimation> jointAnimations = std::vector<JointAnimation>();

	for (unsigned int i = 0; i < animation->mNumChannels; i++) { //every bone
		std::vector<KeyFrame> frames = std::vector<KeyFrame>();

		aiNodeAnim* boneAnim = animation->mChannels[i];
		for (unsigned int j = 0; j < boneAnim->mNumPositionKeys; j++) { //every keyframe of bone
			glm::vec3 pos = glm::vec3(boneAnim->mPositionKeys[j].mValue.x, boneAnim->mPositionKeys[j].mValue.y, boneAnim->mPositionKeys[j].mValue.z);
			glm::quat rot = glm::quat(boneAnim->mRotationKeys[j].mValue.w, boneAnim->mRotationKeys[j].mValue.x, boneAnim->mRotationKeys[j].mValue.y, boneAnim->mRotationKeys[j].mValue.z);
			JointTransform jT = JointTransform(pos, rot);

			KeyFrame kF = KeyFrame(boneAnim->mPositionKeys[j].mTime, jT);
			frames.push_back(kF);
		}

		JointAnimation jA = JointAnimation(boneAnim->mNodeName.C_Str(), frames);
		jointAnimations.push_back(jA);

		numberKeyFrames = std::max(numberKeyFrames, (int)frames.size());
	}

	Animation resultAnim = Animation(animation->mName.C_Str(), length, jointAnimations, numberKeyFrames);

	return resultAnim;
}

void AnimatedModel::loadModel(string const& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	m_globalTransform = scene->mRootNode->mTransformation;

	processNode(scene->mRootNode, scene);

	//normalize joint weights per vertex
	for each (AnimatedMesh m in meshes)
	{
		m.normalizeJointWeights();
	}

	//find root bone

	for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; i++) {
		bool x = findRootBone(scene->mRootNode->mChildren[i], &meshes[i]);
		if (x) {
			break;
		}
	}

	//create bone hierarchy
	createBoneHierarchy(scene);

	testPrintBoneNames();
	//loading animations:
	if (scene->HasAnimations()) {
		for (unsigned int i = 0; i < scene->mNumAnimations; i++) {
			Animation a = loadAnimation(scene->mAnimations[i]);
			animations.push_back(a);
		}
	}
}

void AnimatedModel::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
		
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

AnimatedMesh AnimatedModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<AnimatedVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<AnimatedTexture> textures;

	std::vector<Joint> joints = loadBones(0, mesh);

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		AnimatedVertex vertex;
		glm::vec3 vector;
		vertex.id = i;
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		// texture coordinates
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		if (mesh->mTextureCoords[1]) {
			glm::vec2 lightVec;

			lightVec.x = mesh->mTextureCoords[1][i].x;
			lightVec.y = mesh->mTextureCoords[1][i].y;

			vertex.lightMapCoords = lightVec;
		}
		else {
			vertex.lightMapCoords = glm::vec2(0.5f, 0.5f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	aiMaterial* material = NULL;
	if (mesh->mMaterialIndex >= 0) {
		material = scene->mMaterials[mesh->mMaterialIndex];

		// 1. diffuse maps
		std::vector<AnimatedTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. light maps
		std::vector<AnimatedTexture> lightMaps = loadMaterialTextures(material, aiTextureType_LIGHTMAP, "texture_lightMap");
		textures.insert(textures.end(), lightMaps.begin(), lightMaps.end());
		// 3. normal maps
		std::vector<AnimatedTexture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	float shininess;
	if (AI_SUCCESS != aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess)) {
		shininess = 32.0f;
	}

	aiColor4D ambient;
	if (AI_SUCCESS != aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
		ambient = aiColor4D(0.0f, 0.0f, 0.0f, 1.0f);
	}
	aiColor4D diffuse;
	if (AI_SUCCESS != aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
		diffuse = aiColor4D(0.2f, 0.2f, 0.2f, 1.0f);;
	}
	aiColor4D specular;
	if (AI_SUCCESS != aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)) {
		specular = aiColor4D(0.55f, 0.55f, 0.55f, 1.0f);
	}
	float specularf = (specular.r * 0.2126f + specular.b * 0.0722f + specular.g * 0.7152f);
	float ambientf = (ambient.r * 0.2126f + ambient.b * 0.0722f + ambient.g * 0.7152f);
	float diffusef = (diffuse.r * 0.2126f + diffuse.b * 0.0722f + diffuse.g * 0.7152f);


	vertices = loadJointWeightsToVertices(mesh,vertices);


	AnimatedMesh m = AnimatedMesh(vertices, indices, textures, joints);
	m.setMaterialCoefficient(ambientf, diffusef, specularf, shininess);
	return m;
}

std::vector<Joint> AnimatedModel::loadBones(int meshIndex, const aiMesh* mesh)
{
	std::vector<Joint> joints;
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		int boneIndex = 0;
		string boneName = mesh->mBones[i]->mName.data;
		boneIndex = jointCount;
		jointCount++;
		aiMatrix4x4 offsetMatrix = mesh->mBones[i]->mOffsetMatrix;
		glm::mat4 mat1 = convertToglm(offsetMatrix);
		Joint joint = Joint(boneIndex, boneName, mat1);

		joints.push_back(joint);
	}
	return joints;
}

bool AnimatedModel::findRootBone(aiNode* node, AnimatedMesh* mesh)
{
	string name = node->mName.C_Str();
	string parentName = node->mParent->mName.C_Str();
	std::vector<Joint>* joints = mesh->getJoints();
	for each (Joint joint in *joints)
	{
		if (joint.name == name) { // is bone
			if (!findNameinJoints(parentName, joints)) { //parent is bone
				this->rootJoint = joint;
				return true;
			}
		}
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		bool x = findRootBone(node->mChildren[i], mesh);
		if (x) {
			return true;
		}
	}

	return false;
}

bool AnimatedModel::findNameinJoints(string name, std::vector<Joint>* joints) {
	for each (Joint j in *joints)
	{
		if (name == j.name) {
			return true;
		}
	}
	return false;
}

void AnimatedModel::createBoneHierarchy(const aiScene* scene)
{
	string rootName = this->rootJoint.name;
	aiNode* rootNode = scene->mRootNode;
	std::vector<Joint>* joints = this->meshes[0].getJoints();

	aiNode* rootJointNode = findNodeByName(rootJoint.name, scene->mRootNode);

	for (unsigned int i = 0; i < rootJointNode->mNumChildren; i++) {
		Joint childJoint = getJointbyName(rootJointNode->mChildren[i]->mName.C_Str(), joints);
		rootJoint.addChild(childJoint);
	}
	for each (Joint * var in rootJoint.children) {
		recursiveBoneHierarchy(var, scene);
	}
}

std::vector<AnimatedVertex> AnimatedModel::loadJointWeightsToVertices(aiMesh* mesh, std::vector<AnimatedVertex> vertices)
{
	

	int WEIGHTS_PER_VERTEX = 4;
	//DEFINING HOW MAY WEIGHTS PER VERTEX,
	  //BUT ALSO HOW MANY INDICES PER VERTEX.

	//FIRST YOU HAVE TO CREATE TWO ARRAYS,
	//THESE ARE WHERE WE'LL STORE ALL OF THE VERTEX DATA.
	int boneArraysSize = mesh->mNumVertices * WEIGHTS_PER_VERTEX;
	//THIS IS DONE BECAUSE EACH VERTEX HAS SPACE FOR 4 WEIGHTS AND 4 INDICES.
	std::vector<int> boneIDs;
	boneIDs.resize(boneArraysSize);
	//HERE SPACE FOR 4 INDICES PER VERTEX IS BEING ALLOCATED
	std::vector<float> boneWeights;
	boneWeights.resize(boneArraysSize);
	//HERE SPACE FOR 4 WEIGHTS PER VERTEX IS BEING ALLOCATED

//HERE WE FILL THE ARRAYS, (below)
//WE DO THIS BY CYCLING THROUGH EACH BONE AND ITS DATA,
//AND COPYING IT INTO ITS RESPECTIVE ARRAY.
	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		//(above) NOTE THAT mesh IS NOT OF TYPE Mesh,
		//IT IS A POINTER TO THE CURRENT MESH, OF TYPE aiMesh

		aiBone* aiBone = mesh->mBones[i]; //CREATING A POINTER TO THE CURRENT BONE
		//IT'S IMPORTANT TO NOTE THAT i IS JUST THE ID OF THE CURRENT BONE.

		for (unsigned int j = 0; j < aiBone->mNumWeights; j++)
		{
			aiVertexWeight weight = aiBone->mWeights[j];

			//THIS WILL TELL US WHERE, IN OUR ARRAY, TO START READING THE VERTEX'S WEIGHTS
			unsigned int vertexStart = weight.mVertexId * WEIGHTS_PER_VERTEX;

			//HERE WE'LL ACTUALLY FILL THE ARRAYS, WITH BOTH INDICES AND WEIGHTS.
			for (int k = 0; k < WEIGHTS_PER_VERTEX; k++)
			{
				if (boneWeights.at(vertexStart + k) == 0)
				{
					//(above) IF THE CURRENT BONE WEIGHT IS EQUAL TO 0,
					//THEN IT HASN'T BEEN FILLED YET WITH AN ACTUAL WEIGHT.
					boneWeights.at(vertexStart + k) = weight.mWeight;
					boneIDs.at(vertexStart + k) = i; //REMEMBER THAT i IS JUST THE ID OF THE CURRENT BONE.

					//NOTE THAT data IS JUST AN ARRAY OF TYPE Vertex, WHERE I STORE ALL OF THE VERTEX INFO.
					//EACH Vertex CLASS HAS SPACE FOR A POSITION, A UV, A NORMAL, AND 4 INDICES, AND 4 WEIGHTS.
					//EACH Mesh IS THEN CREATED WITH THIS THIS ARRAY OF Vertex (THIS ARRAY BEING data).

					vertices.at(weight.mVertexId).jointIDs[k] = i;
					//SETTING THE ID
					//AT k, OF
					//THE VERTEX AT THIS WEIGHT'S ID,
					//TO THE CURRENT BONE ID.

					vertices.at(weight.mVertexId).jointWeights[k] = weight.mWeight;
					//SETTING THE WEIGHT
					//AT k, OF
					//THE VERTEX AT THIS WEIGHT'S ID,
					//TO THIS WEIGHT'S WEIGHT.
					break;
				}
			}
		}
	}

	return vertices;
}

void AnimatedModel::recursiveBoneHierarchy(Joint* parentJoint, const aiScene* scene) {
	std::vector<Joint>* joints = this->meshes[0].getJoints();
	aiNode* parentJointNode = findNodeByName(parentJoint->name, scene->mRootNode);

	for (unsigned int i = 0; i < parentJointNode->mNumChildren; i++) {
		Joint childJoint = getJointbyName(parentJointNode->mChildren[i]->mName.C_Str(), joints);
		parentJoint->addChild(childJoint);
	}

	for each (Joint * j in parentJoint->children)
	{
		recursiveBoneHierarchy(j, scene);
	}
}

aiNode* AnimatedModel::findNodeByName(string name, aiNode* rootNode)
{
	if (rootNode->mName.C_Str() == name) {
		return rootNode;
	}

	for (unsigned int i = 0; i < rootNode->mNumChildren; i++) {
		aiNode* n = findNodeByName(name, rootNode->mChildren[i]);
		if (n) {
			return n;
		}
	}

	return nullptr;
}

Joint AnimatedModel::getJointbyName(string name, std::vector<Joint>* joints)
{
	for each (Joint j in *joints)
	{
		if (j.name == name) {
			return j;
		}
	}
	return Joint();
}

void AnimatedModel::addJointsToArray(Joint headJoint, std::vector<glm::mat4>* jointMatrices)
{
	jointMatrices->insert(jointMatrices->begin() + headJoint.id, headJoint.getAnimatedTransform());
	for each (Joint * childJoint in headJoint.children)
	{
		addJointsToArray(*childJoint, jointMatrices);
	}
}

AnimatedModel::AnimatedModel()
{
	animator = nullptr;
}

AnimatedModel::AnimatedModel(string const& path, glm::vec3 position, glm::vec3 scale)
{
	this->directory = path;
	this->setPosition(position);
	this->transform.setScale(scale);

	loadModel(path);

	this->animator = new Animator(this);
	doAnimation(Animation::WALK);
	rootJoint.calcInverseBindTransform(glm::mat4());
}

std::vector<glm::mat4> AnimatedModel::getJointTransforms()
{
	std::vector<glm::mat4>* jointMatrices = new std::vector<glm::mat4>();
	jointMatrices->resize(jointCount);

	addJointsToArray(rootJoint, jointMatrices);
	return *jointMatrices;
}

void AnimatedModel::setUniforms(AdvancedShader* shader)
{
	shader->setUniform("modelMatrix", transform.getModelMatrix());
	std::vector<glm::mat4> jointTransforms = getJointTransforms();

	for (GLint i = 0; i < jointTransforms.size(); i++) {
		string number = std::to_string(i);
		shader->setUniform(("jointTransforms[" + number + "]").c_str(), jointTransforms.at(i));
	}
}

std::vector<AnimatedTexture> AnimatedModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
	std::vector<AnimatedTexture> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)

				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			AnimatedTexture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = (str);
			textures.push_back(texture);

			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

unsigned int AnimatedModel::TextureFromFile(const char* path, const string& directory)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* data = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(data);

	return textureID;
}

void AnimatedModel::draw(AdvancedShader* shader)
{
	setUniforms(shader);
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes.at(i).draw(*shader);
	}
}

void AnimatedModel::doAnimation(Animation::ANIMATION anim)
{
	switch (anim)
	{
	case Animation::WALK:
		animator->doAnimation(getAnimation("walk"));
		break;
	case Animation::RUN:
		animator->doAnimation(getAnimation("run"));
		break;
	default:
		break;
	}
}

void AnimatedModel::update(float dt)
{
	animator->update(dt, &this->rootJoint);
}

glm::mat4 AnimatedModel::convertToglm(aiMatrix4x4 aiMat) {
	glm::mat4 mat = glm::mat4(0);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			mat[i][j] = aiMat[i][j];
		}
	}

	return mat;
}

void AnimatedModel::testPrintBoneNames()
{
	rootJoint.printName();
}

Animation AnimatedModel::getAnimation(string name)
{
	return animations.at(0);
}
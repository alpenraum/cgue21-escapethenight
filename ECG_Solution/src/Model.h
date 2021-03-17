#pragma once

#include "Actor.h"
#include "Shader.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SOIL2/SOIL2.h>
#include "AdvancedShader.h"





class Model :
	public Actor
{
public:
	Model();
	Model(string const &path, glm::vec3 position);
	Model(string const &path, glm::vec3 position, glm::vec3 scale);
	virtual void draw(AdvancedShader &shader);
	void setMaterialCoefficients(float ambient, float diffuse, float specular);
	
	void setScale(float coeff) {
		this->transform.setScale(glm::vec3(coeff));
	}
	void setShininess(float shininess) {
		for each (Mesh mesh in meshes)
		{
			mesh.setShininess(shininess);
		}
	}

	glm::vec2 getBottomLeft();
	glm::vec2 getTopRight();
protected:
	//saves all Meshes from this Model
	std::vector<Mesh> meshes;
	//saves all already loaded Textures from this Model
	std::vector<TestTexture>textures_loaded;
	//The directory in which the model file is saved
	string directory;



	

	/*
	loads a Model from a file
	@param path - path to the file which should be loaded
	*/
	void loadModel(string const &path);

	/*
	Processes a node from a scene (i.e. retrieves the meshes from each node)
	Calls itself recursively until all child-nodes are processed aswell
	@param node - the node which is going to get processed (first call should always happen with the root-Node
	@param scene - aiScene in which all nodes are saved
	*/
	void processNode(aiNode *node, const aiScene *scene);
	/*
	Processes a mesh (i.e. retrieves all vertices, indices and textures)
	@param mesh - aiMesh which is going to be processed (gotten from a aiScene via the corresponding index saved in a aiNode)
	@param scene - Scene in which this Mesh is saved
	@return Mesh object 
	*/
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	/*
	loads all Textures of a Material (each aiMesh has one aiMaterial)
	if a Texture is already loaded it will be skipped (to enhance performance)
	@param mat -  aiMaterial of the corresponding aiMesh of which all Textures are being loaded
	@param type - The purpose of a Texture being loaded. (e.g. if type = aiTextureType_DIFFUSE, only the diffuse textures of this Material will be loaded)
	@param typeName - designation of this aiTextureType which is saved inside a Texture object (don't quite know what this is used for; probably just being able to distinguish the different types of texture objects)
	@return Vector with all loaded textures
	*/
	std::vector<TestTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

	unsigned int TextureFromFile(const char *path, const string &directory);

	
	

};

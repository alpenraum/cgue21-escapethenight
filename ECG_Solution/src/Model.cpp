#include "Model.h"
#include "utils/Converter.cpp"

Model::Model() {

}
Model::Model(string const &path, glm::vec3 position) {
	this->transform.setPosition(position);
	loadModel(path);
}
Model::Model(string const &path, glm::vec3 position, glm::vec3 scale) {
	this->transform.setPosition(position);
	this->transform.setScale(scale);
	loadModel(path);
}

void Model::draw(AdvancedShader &shader) {
	shader.setUniform("alpha", 1.0f);
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader, transform);
		
	}
}



void Model::loadModel(string const &path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

	

}

glm::mat4 Model::convertToglm(aiMatrix4x4 aiMat) {
	glm::mat4 to;

	//ROW MAJOR

	//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
	to[0][0] = aiMat.a1; to[1][0] = aiMat.a2; to[2][0] = aiMat.a3; to[3][0] = aiMat.a4;
	to[0][1] = aiMat.b1; to[1][1] = aiMat.b2; to[2][1] = aiMat.b3; to[3][1] = aiMat.b4;
	to[0][2] = aiMat.c1; to[1][2] = aiMat.c2; to[2][2] = aiMat.c3; to[3][2] = aiMat.c4;
	to[0][3] = aiMat.d1; to[1][3] = aiMat.d2; to[2][3] = aiMat.d3; to[3][3] = aiMat.d4;
	return to;
}

void Model::processNode(aiNode *node, const aiScene *scene) {

	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TestTexture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;

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
		std::vector<TestTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	float shininess;
	if (AI_SUCCESS != aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess)) {
		shininess = 32.0f;
	}
	
	aiColor4D ambient;
		if (AI_SUCCESS != aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
			ambient = aiColor4D(0.0f,0.0f,0.0f,1.0f);
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
	Mesh m = Mesh(vertices, indices, textures);
	m.setMaterialCoefficient(ambientf,diffusef,specularf,shininess);
	return m;
}


std::vector<TestTexture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
	std::vector<TestTexture> textures;
	
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++){
			if (textures_loaded[j].path == str)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)

				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			TestTexture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type= typeName;
			texture.path= (str);
			textures.push_back(texture);

			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;

}

 unsigned int Model::TextureFromFile(const char *path, const string &directory)
{
	string filename = string(path);
	filename = directory + '/' + filename;

	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char *data = SOIL_load_image(filename.c_str(), &width, &height, 0,SOIL_LOAD_RGBA);
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

 
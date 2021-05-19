#pragma once
#include "Actor.h"
#include <string>
#include <sstream>
#include <iostream>
#include <assimp/Importer.hpp>
#include "AdvancedShader.h"
#include <iostream>
#include <GL/glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\euler_angles.hpp>
#include "utils/Transform.h"

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
	glm::vec2 lightMapCoords;
	glm::vec4 jointWeights;
	glm::ivec4 jointIDs;
	int id;
};

struct TestTexture {
	GLuint id;
	string type;
	aiString path;
};

/*!
* A Mesh is an Actor that has geometry, one or more textures, a shader and a bounding box or sphere. Every Mesh can be rendered.
*/
class Mesh

{
protected:
	//stores the vertices of a Mesh
	std::vector<Vertex> vertices;
	//stores the indices of a Mesh
	std::vector<GLuint> indices;
	//stores the textures of a Mesh
	std::vector<TestTexture> textures;

	//all Buffer Objects
	GLuint VAO, VBO, EBO;

	//material coefficient
	float ambient, diffuse, specular, shininess;

	/*
	creates all buffer objects, binds the vertices, indices, and texture-Coordinates to the Vertex-Array-Object
	so it can be passed to the gpu when this Mesh is drawn
	*/
	void setupMesh();
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<TestTexture>& textures);
	Mesh();
	~Mesh();


	
	/*
	draws a Mesh
	@param shader - Shader which is used for drawing this mesh
	*/
	void draw(AdvancedShader& shader);

	void draw(AdvancedShader& shader, Transform tm);

	std::vector<Vertex> getVertices() {
		return vertices;
	}

	void setVertices(std::vector<Vertex> vertices) {
		this->vertices = vertices;
	}
	std::vector<GLuint> getIndices() {
		return indices;
	}

	TestTexture getFirstTexture() {
		return textures[0];
	}

	void setMaterialCoefficient(float ambient, float diffuse, float specular, float shininess);
	void setShininess(float shininess) {
		this->shininess = shininess;
	}
};

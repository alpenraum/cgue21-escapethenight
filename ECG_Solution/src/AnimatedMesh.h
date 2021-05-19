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
#include "Joint.h"


struct AnimatedVertex {
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
	glm::vec2 lightMapCoords;
	glm::vec4 jointWeights;
	glm::ivec4 jointIDs;
	int id;
};

struct AnimatedTexture {
	GLuint id;
	string type;
	aiString path;
};
class AnimatedMesh :
    public Actor
{
private:
	//stores the vertices of a Mesh
	std::vector<AnimatedVertex> vertices;
	//stores the indices of a Mesh
	std::vector<GLuint> indices;
	//stores the textures of a Mesh
	std::vector<AnimatedTexture> textures;

    std::vector<Joint> joints;

	//all Buffer Objects
	GLuint VAO, VBO, EBO;

	//material coefficient
	float ambient, diffuse, specular, shininess;

    void setupMesh();
public:
    AnimatedMesh();
    AnimatedMesh(std::vector<AnimatedVertex>& vertices, std::vector<GLuint>& indices, std::vector<AnimatedTexture>& textures, std::vector<Joint>& joints);
    std::vector<Joint>* getJoints() {
        return &joints;
    }

    void normalizeJointWeights();

	/*
	draws a Mesh
	@param shader - Shader which is used for drawing this mesh
	*/
	void draw(AdvancedShader& shader);

	void draw(AdvancedShader& shader, Transform tm);

	std::vector<AnimatedVertex> getVertices() {
		return vertices;
	}

	void setVertices(std::vector<AnimatedVertex> vertices) {
		this->vertices = vertices;
	}
	std::vector<GLuint> getIndices() {
		return indices;
	}

	AnimatedTexture getFirstTexture() {
		return textures[0];
	}

	void setMaterialCoefficient(float ambient, float diffuse, float specular, float shininess);
	void setShininess(float shininess) {
		this->shininess = shininess;
	}

   
};


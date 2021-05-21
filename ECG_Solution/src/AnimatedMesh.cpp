#include "AnimatedMesh.h"

void AnimatedMesh::setupMesh()
{



	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(AnimatedVertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)0);

	//vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, normal));

	// vertex tex Coordinates
		glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, texCoords));


	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, jointIDs));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (GLvoid*)offsetof(AnimatedVertex, jointWeights));

	//unbind
	glBindVertexArray(0);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
}

AnimatedMesh::AnimatedMesh()
{
}

AnimatedMesh::AnimatedMesh(std::vector<AnimatedVertex>& vertices, std::vector<GLuint>& indices, std::vector<AnimatedTexture>& textures, std::vector<Joint>& joints)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->joints = joints;
	this->ambient = 0.0f;
	this->diffuse = 0.0f;
	this->specular = 0.0f;

	setupMesh();
}


void AnimatedMesh::normalizeJointWeights()
{
	//for each (AnimatedVertex v in vertices) {
		//v.jointWeights = glm::normalize(v.jointWeights);
	//}
}

void AnimatedMesh::draw(AdvancedShader& shader)
{
	GLuint diffuseNr = 1;

	for (GLint i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		std::stringstream ss;
		string number;
		string name = textures[i].type;

		if (name == "texture_diffuse") {
			ss << diffuseNr++;
		}
		
		number = ss.str();

		shader.setUniform((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	shader.setUniform("materialCoefficients", glm::vec3(ambient, diffuse, specular));
	shader.setUniform("shininess", shininess);

	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	//unbind everything
	glBindVertexArray(0);
	for (GLuint i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void AnimatedMesh::draw(AdvancedShader& shader, Transform tm)
{
	shader.setUniform("modelMatrix", tm.getModelMatrix());
	shader.setUniform("normalMatrix", glm::transpose(glm::inverse(tm.getModelMatrix())));
	draw(shader);
}

void AnimatedMesh::setMaterialCoefficient(float ambient, float diffuse, float specular, float shininess)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	this->shininess = shininess;
}

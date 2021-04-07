#pragma once
#include "utils/Utils.h"
#include "AdvancedShader.h"
#include "BasicCamera.h"
#include "Particle.h"
class ParticleRenderer {
private:
	const int MAX_INSTANCES = 10000;
	const int INSTANCE_DATA_LENGTH = 16+1+3; //16 floats of the modelViewMatrix(4x4) + 1 float alpha + 3 floats hue

	std::vector<float> floatBuffer;
	int bufferPointer = 0;

	std::shared_ptr<AdvancedShader> shader;

	GLuint particleVAO, particleVBO, instanceVBO;
	GLuint particleTexture;




	float quadVertices[20] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix);
	void updateVBO(int vbo, std::vector<float> data);
	void addInstancedAttribute(GLuint vbo, GLuint attribute, int dataSize, int instancedDataLength, int offset);
public:

	ParticleRenderer();

	void draw(ICamera* camera, float dt, std::vector<Particle*> particles);

	void cleanup();
	
};

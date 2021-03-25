#pragma once
#include "utils/Utils.h"
#include "AdvancedShader.h"
#include "BasicCamera.h"
#include "Particle.h"
class ParticleRenderer {
private:
	const int MAX_INSTANCES = 10000;
	const int INSTANCE_DATA_LENGTH = 16; //16 floats of the modelViewMatrix(4x4)

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
	GLuint createEmptyVBO(int floatCount);
	void addInstanceAttribute(int attribute, int dataSize, int instancedDataLength, int offset);
	void updateVBO(std::vector<float> data);
public:

	ParticleRenderer();

	void draw(ICamera* camera, float dt, std::vector<Particle*> particles);

	void cleanup();
	
};

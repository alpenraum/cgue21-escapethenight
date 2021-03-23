#pragma once
#include "utils/Utils.h"
#include "AdvancedShader.h"
#include "BasicCamera.h"
#include "Particle.h"
class ParticleRenderer {
private:
	std::shared_ptr<AdvancedShader> shader;

	GLuint particleVAO, particleVBO;

	float quadVertices[20] = {
		// positions        // texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	};

	void updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix);

public:

	ParticleRenderer();

	void draw(ICamera* camera, float dt, std::vector<Particle*> particles);

	void cleanup();
};

#include "ParticleRenderer.h"
#include "utils/TextureLoader.h"
//Particle texture source: https://www.kenney.nl/assets/particle-pack

glm::mat4 ParticleRenderer::updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix)
{
	glm::mat3 invRotMatrix = glm::inverse(glm::mat3(viewMatrix));
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);

	modelMatrix[0][0] = viewMatrix[0][0];
	modelMatrix[0][1] = viewMatrix[1][0];
	modelMatrix[0][2] = viewMatrix[2][0];
	modelMatrix[1][0] = viewMatrix[0][1];
	modelMatrix[1][1] = viewMatrix[1][1];
	modelMatrix[1][2] = viewMatrix[2][1];
	modelMatrix[2][0] = viewMatrix[0][2];
	modelMatrix[2][1] = viewMatrix[1][2];
	modelMatrix[2][2] = viewMatrix[2][2];

	//modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

	return modelViewMatrix;
}

ParticleRenderer::ParticleRenderer()
{
	shader = std::make_shared<AdvancedShader>("particle.vert", "particle.frag");

	glGenVertexArrays(1, &particleVAO);
	glGenBuffers(1, &particleVBO);
	glBindVertexArray(particleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	//vertex coords
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//tex coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(glm::mat4), 0, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	particleTexture = TextureLoader::loadTexture("assets/particles/fireParticle_alpha.png");

	glBindTexture(GL_TEXTURE_2D, particleTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ParticleRenderer::draw(ICamera* camera, float dt, std::vector<Particle*> particles)
{
	glm::mat4 view = camera->getViewMatrix();

	shader->use();
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particleTexture);
	shader->setUniform("particleTexture", 0);

	glBindVertexArray(particleVAO);

	shader->setUniform("projectionMatrix", camera->getProjMatrix());

	std::vector<glm::mat4>* matrices = new std::vector<glm::mat4>();

	for each (Particle * p in particles)
	{
		matrices->push_back(updateModelViewMatrix(p->getPosition(), p->getRotation(), p->getScale(), view));
	}

	// vertex attributes

	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, matrices->size() * sizeof(glm::mat4), &matrices[0], GL_DYNAMIC_DRAW);

	std::size_t vec4Size = sizeof(glm::vec4);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	

	for each (Particle * p in particles)
	{
		if (p->getDistToCamera() < Settings::particleRenderDistance * Settings::particleRenderDistance) {
			shader->setUniform("alpha", p->getAlpha());
			shader->setUniform("hue", p->getHue());
		}
	}
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particles.size());

	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader->unuse();
}

void ParticleRenderer::cleanup()
{
	glDeleteProgram(shader->getProgramId());
}

GLuint ParticleRenderer::createEmptyVBO(int floatCount) {
	return 0;
}

void ParticleRenderer::addInstanceAttribute(int attribute, int dataSize, int instancedDataLength, int offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBindVertexArray(particleVAO);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, false, instancedDataLength * sizeof(float), (void*)(offset * sizeof(float)));
	glVertexAttribDivisor(attribute, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ParticleRenderer::updateVBO(std::vector<float>* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, data->size() * sizeof(float), &data, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
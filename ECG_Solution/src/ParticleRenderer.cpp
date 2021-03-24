#include "ParticleRenderer.h"

void ParticleRenderer::updateModelViewMatrix(glm::vec3 position, float rotation, float scale, glm::mat4 viewMatrix)
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
	
	
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
	
	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

	shader->setUniform("modelViewMatrix", modelViewMatrix);
}

ParticleRenderer::ParticleRenderer()
{
	shader = std::make_shared<AdvancedShader>("particle.vert", "particle.frag");



	glGenVertexArrays(1, &particleVAO);
	glGenBuffers(1, &particleVBO);
	glBindVertexArray(particleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void ParticleRenderer::draw(ICamera* camera, float dt, std::vector<Particle*> particles)
{
	glm::mat4 view = camera->getViewMatrix();
		
	shader->use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	glBindVertexArray(particleVAO);
	int c = 0;
	shader->setUniform("projectionMatrix", camera->getProjMatrix());
	for each (Particle* p in particles)
	{
		c++;
		shader->setUniform("alpha", p->getAlpha());
		shader->setUniform("hue", p->getHue());
		updateModelViewMatrix(p->getPosition(), p->getRotation(), p->getScale(), view);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	
	

	

	glBindVertexArray(0);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	shader->unuse();
}

void ParticleRenderer::cleanup()
{
	glDeleteProgram(shader->getProgramId());
}

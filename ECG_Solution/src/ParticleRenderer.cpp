#include "ParticleRenderer.h"
#include "utils/TextureLoader.h"
//Particle texture source: https://www.kenney.nl/assets/particle-pack


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
	
	
	//modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

	floatBuffer[bufferPointer++] = modelViewMatrix[0][0];
	floatBuffer[bufferPointer++] = modelViewMatrix[0][1];
	floatBuffer[bufferPointer++] = modelViewMatrix[0][2];
	floatBuffer[bufferPointer++] = modelViewMatrix[0][3];
	floatBuffer[bufferPointer++] = modelViewMatrix[1][0];
	floatBuffer[bufferPointer++] = modelViewMatrix[1][1];
	floatBuffer[bufferPointer++] = modelViewMatrix[1][2];
	floatBuffer[bufferPointer++] = modelViewMatrix[1][3];
	floatBuffer[bufferPointer++] = modelViewMatrix[2][0];
	floatBuffer[bufferPointer++] = modelViewMatrix[2][1];
	floatBuffer[bufferPointer++] = modelViewMatrix[2][2];
	floatBuffer[bufferPointer++] = modelViewMatrix[2][3];
	floatBuffer[bufferPointer++] = modelViewMatrix[3][0];
	floatBuffer[bufferPointer++] = modelViewMatrix[3][1];
	floatBuffer[bufferPointer++] = modelViewMatrix[3][2];
	floatBuffer[bufferPointer++] = modelViewMatrix[3][3];

}

void ParticleRenderer::updateVBO(int vbo, std::vector<float> data)
{
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float),NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float),&data[0]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

ParticleRenderer::ParticleRenderer()
{

	floatBuffer = std::vector<float>();
	floatBuffer.resize(MAX_INSTANCES * INSTANCE_DATA_LENGTH);

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


	//instance vbo
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * INSTANCE_DATA_LENGTH * sizeof(float),NULL, GL_DYNAMIC_DRAW);

	//column a of viewMatrix
	addInstancedAttribute(instanceVBO, 2, 4, INSTANCE_DATA_LENGTH, 0);
	//column b of viewMatrix
	addInstancedAttribute(instanceVBO, 3, 4, INSTANCE_DATA_LENGTH, 4);
	//column c of viewMatrix
	addInstancedAttribute(instanceVBO, 4, 4, INSTANCE_DATA_LENGTH, 8);
	//column d of viewMatrix
	addInstancedAttribute(instanceVBO, 5, 4, INSTANCE_DATA_LENGTH, 12);
	//vec3 hue
	addInstancedAttribute(instanceVBO, 6, 3, INSTANCE_DATA_LENGTH, 16);
	//float alpha
	addInstancedAttribute(instanceVBO, 7, 1, INSTANCE_DATA_LENGTH, 19);

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

	bufferPointer = 0;
	
	for each (Particle * p in particles)
	{
		if (p->getDistToCamera() < Settings::particleRenderDistance * Settings::particleRenderDistance) {
			
			updateModelViewMatrix(p->getPosition(), p->getRotation(), p->getScale(), view);
			//hue
			floatBuffer[bufferPointer++] = p->getHue().x;
			floatBuffer[bufferPointer++] = p->getHue().y;
			floatBuffer[bufferPointer++] = p->getHue().z;
			//alpha
			floatBuffer[bufferPointer++] = p->getAlpha();
		}
	}
	
	updateVBO(instanceVBO, floatBuffer);
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

void ParticleRenderer::addInstancedAttribute(GLuint vbo, GLuint attribute, int dataSize, int instancedDataLength, int offset) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(particleVAO);

	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, instancedDataLength * sizeof(float), (void*)(offset * sizeof(float)));
	glVertexAttribDivisor(attribute, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



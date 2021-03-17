#include "OmniShadowRenderer.h"

OmniShadowRenderer::OmniShadowRenderer(string vf, string gf, string ff)
{
	shader = std::make_shared<AdvancedShader>("shadow.vert", "shadow.frag", "shadow.geom");
	shader->use();
}

OmniShadowRenderer::OmniShadowRenderer() : OmniShadowRenderer("shadow.vert", "shadow.frag", "shadow.geom")
{
}

void OmniShadowRenderer::prepareRender(PointLight* pointLight)
{
	shader->use();

	shader->setUniform("lightPos", pointLight->position);
	shader->setUniform("farPlane", Settings::farPlane);

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)Settings::shadowTextureDimension / (float)Settings::shadowTextureDimension, 1.0f, Settings::shadowDistance);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLight->position, pointLight->position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLight->position, pointLight->position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLight->position, pointLight->position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLight->position, pointLight->position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLight->position, pointLight->position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(pointLight->position, pointLight->position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

	for (unsigned int i = 0; i < 6; ++i) {
		shader->setUniform("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}

	pointLight->omniShadowFBO.bindFBO();
	glViewport(0, 0, Settings::shadowTextureDimension, Settings::shadowTextureDimension);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OmniShadowRenderer::cleanUpAfterRender(PointLight* pointLight)
{
	pointLight->omniShadowFBO.unbindFBO();
	shader->unuse();
}

void OmniShadowRenderer::cleanup()
{
	glDeleteProgram(shader->getProgramId());
}
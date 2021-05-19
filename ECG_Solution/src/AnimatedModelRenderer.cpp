#include "AnimatedModelRenderer.h"

AnimatedModelRenderer::AnimatedModelRenderer()
{
	this->shader = std::make_shared<AdvancedShader>("animatedModelVertex.vert", "newFrag.frag");
	shader->use();
	shader->unuse();
}

void AnimatedModelRenderer::render(AnimatedModel* entity, ICamera* camera)
{
	entity->draw(shader.get());
	finish();
}

void AnimatedModelRenderer::prepareRender(ICamera* camera, glm::vec4 clippingPlane, bool lightMapping, bool normalMapping, std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights)
{
	shader->use();
	shader->setUniform("viewProjMatrix", camera->getViewProjMatrix());
	shader->setUniform("clippingPlane", clippingPlane);
	shader->setUniform("cameraWorld", camera->getPosition());
	shader->setUniform("alpha", 1.0f);
	shader->setUniform("gamma", Settings::gamma);
	shader->setUniform("lightMapping", lightMapping);
	shader->setUniform("normaMapping", normalMapping);
	shader->setUniform("farPlane", Settings::farPlane);
	for (GLint i = 0; i < dirLights.size(); i++) {
		string number = std::to_string(i);
		shader->setUniform(("dirLights[" + number + "].color").c_str(), dirLights.at(i)->color);
		shader->setUniform(("dirLights[" + number + "].direction").c_str(), dirLights.at(i)->direction);
	}

	for (GLint i = 0; i < pointLights.size(); i++) {
		PointLight* temp = pointLights.at(i);

		string number = std::to_string(i);
		shader->setUniform(("pointLights[" + number + "].color").c_str(), temp->color);
		shader->setUniform(("pointLights[" + number + "].position").c_str(), temp->position);
		shader->setUniform(("pointLights[" + number + "].attentuation").c_str(), temp->attenuation);
		shader->setUniform(("pointLights[" + number + "].enabled").c_str(), temp->enabled);
		shader->setUniform(("pointLights[" + number + "].emittingShadows").c_str(), temp->castsShadows());
		glActiveTexture(GL_TEXTURE0 + 3 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, temp->omniShadowFBO.getDepthMap());
		shader->setUniform(("pointLights[" + number + "].depthMap").c_str(), 0 + 3 + i);
	}
}

void AnimatedModelRenderer::finish()
{
	shader->unuse();
}
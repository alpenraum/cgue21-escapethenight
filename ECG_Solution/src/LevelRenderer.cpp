#include "LevelRenderer.h"

void LevelRenderer::addModel(string path, glm::vec3 position, glm::vec3 scale)
{
	Model* model = new Model(path, position, scale);

	modelList.push_back(model);
}

void LevelRenderer::addModel(Model* model)
{
	modelList.push_back(model);
}

void LevelRenderer::addModels(std::vector<Model*> models)
{
	modelList = models;
}

void LevelRenderer::render()
{
	shader->use();
	for each (Model * model in modelList) {
		model->draw(*shader.get());
	}
	shader->unuse();
}

void LevelRenderer::setUniforms(bool shadows, ICamera* camera, glm::vec4 clippingPlane, bool lightMapping, bool normalMapping, std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights, GLuint depthCubemap)
{
	AdvancedShader* s;

	if (shadows) {
		s = shader.get();
	}
	else {
		s = shadowlessShader.get();
	}
	s->use();
	s->setUniform("viewProjMatrix", camera->getViewProjMatrix());
	s->setUniform("clippingPlane", clippingPlane);
	s->setUniform("cameraWorld", camera->getPosition());
	s->setUniform("alpha", 1.0f);
	s->setUniform("gamma", Settings::gamma);
	s->setUniform("farPlane", Settings::farPlane);

	for (GLint i = 0; i < dirLights.size(); i++) {
		string number = std::to_string(i);
		s->setUniform(("dirLights[" + number + "].color").c_str(), dirLights.at(i)->color);
		s->setUniform(("dirLights[" + number + "].direction").c_str(), dirLights.at(i)->direction);
	}

	for (GLint i = 0; i < pointLights.size(); i++) {
		PointLight* temp = pointLights.at(i);

		string number = std::to_string(i);
		s->setUniform(("pointLights[" + number + "].color").c_str(), temp->color);
		s->setUniform(("pointLights[" + number + "].position").c_str(), temp->position);
		s->setUniform(("pointLights[" + number + "].attentuation").c_str(), temp->attenuation);
		s->setUniform(("pointLights[" + number + "].enabled").c_str(), temp->enabled);
		s->setUniform(("pointLights[" + number + "].emittingShadows").c_str(), temp->castsShadows());
		glActiveTexture(GL_TEXTURE0 + 3 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, temp->omniShadowFBO.getDepthMap());
		s->setUniform(("pointLights[" + number + "].depthMap").c_str(), 0 + 3 + i);
	}
}

LevelRenderer::LevelRenderer()
{
	shader = std::make_shared<AdvancedShader>("newVert.vert", "newFrag.frag");
	shader->use();
	shader->unuse();

	shadowlessShader = std::make_shared<AdvancedShader>("newVertNoShadows.vert", "newFragNoShadows.frag");
	shadowlessShader->use();
	shadowlessShader->unuse();

	modelList = std::vector<Model*>();
}

/*
* Renders the objects in the modelList
*
* setUniforms() must be called before!
*/
void LevelRenderer::renderWithoutShadows()
{
	shadowlessShader->use();
	for each (Model * model in modelList) {
		model->draw(*shadowlessShader.get());
	}
	shadowlessShader->unuse();
}

void LevelRenderer::cleanup()
{
	glDeleteProgram(shader->getProgramId());
}
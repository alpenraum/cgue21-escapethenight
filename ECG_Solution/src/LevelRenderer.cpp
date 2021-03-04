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



void LevelRenderer::setUniforms(ICamera* camera, glm::vec4 clippingPlane, bool lightMapping, bool normalMapping, std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights)
{	
	shader->use();
	shader->setUniform("viewProjMatrix", camera->getViewProjMatrix());
	shader->setUniform("clippingPlane", clippingPlane);
	shader->setUniform("cameraWorld", camera->getPosition());
	shader->setUniform("alpha", 1.0f);
	shader->setUniform("gamma", Settings::gamma);
	shader->setUniform("lightMapping", lightMapping);
	shader->setUniform("normaMapping", normalMapping);

	for (GLuint i = 0; i < dirLights.size(); i++) {
		string number = std::to_string(i);
		shader->setUniform(("dirLights[" + number + "].color").c_str(), dirLights.at(i)->color);
		shader->setUniform(("dirLights[" + number + "].direction").c_str(), dirLights.at(i)->direction);
	}

	for (GLuint i = 0; i < pointLights.size(); i++) {
		PointLight* temp = pointLights.at(i);

		string number = std::to_string(i);
		shader->setUniform(("pointLights[" + number + "].color").c_str(), temp->color);
		shader->setUniform(("pointLights[" + number + "].position").c_str(), temp->position);
		shader->setUniform(("pointLights[" + number + "].attentuation").c_str(), temp->attenuation);
		shader->setUniform(("pointLights[" + number + "].enabled").c_str(), temp->enabled);
	}
}

LevelRenderer::LevelRenderer()
{

	std::vector<string> uniformNames = std::vector<string>();
	uniformNames.push_back("viewProjMatrix");
	uniformNames.push_back("clippingPlane");
	uniformNames.push_back("cameraWorld");
	uniformNames.push_back("alpha");
	uniformNames.push_back("gamma");
	uniformNames.push_back("lightMapping");
	uniformNames.push_back("normalMapping");
	

	shader = std::make_shared<Shader>("newVert.vert", "newFrag.frag");
	shader->use();
	
	shader->initUniforms(uniformNames);

	shader->unuse();

	modelList = std::vector<Model*>();

}

/*
* Renders the objects in the modelList
* 
* setUniforms() must be called before!
*/
void LevelRenderer::render()
{
	shader->use();
	for each (Model * model in modelList) {
		model->draw(*shader.get());
	}
	shader->unuse();
}

void LevelRenderer::cleanup()
{
	
}

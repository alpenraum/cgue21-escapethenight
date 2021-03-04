#pragma once
#include "utils/Utils.h"
#include "Shader.h"
#include "Model.h"
#include "ICamera.h"
#include "utils/Settings.h"
#include "Light.h"
class LevelRenderer
{
private:
	

	std::shared_ptr<Shader> shader;
	std::vector<Model*>  modelList;


	
public:
	LevelRenderer();

	void setUniforms(ICamera* camera, glm::vec4 clippingPlane, bool lightMapping, bool normalMapping, std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights);

	void addModel(string path, glm::vec3 position, glm::vec3 scale);
	void addModel(Model* model);
	void addModels(std::vector<Model*> models);

	void render();

	void cleanup();

	

};


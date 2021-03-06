#pragma once
#include "utils/Utils.h"
#include "AdvancedShader.h"
#include "Model.h"
#include "ICamera.h"
#include "utils/Settings.h"
#include "Light.h"
class LevelRenderer
{
private:
	

	std::shared_ptr<AdvancedShader> shader;
	std::shared_ptr<AdvancedShader> shadowlessShader;
	std::vector<Model*>  modelList;

	
public:
	LevelRenderer();

	
	void setUniforms(bool shadows,ICamera* camera, glm::vec4 clippingPlane, bool lightMapping, bool normalMapping, std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights, GLuint depthCubemap = 0);

	void addModel(string path, glm::vec3 position, glm::vec3 scale);
	void addModel(Model* model);
	void addModels(std::vector<Model*> models);

	std::vector<Model*>* getModels() { return &modelList; }
	AdvancedShader* getShader() { return shader.get(); }
	AdvancedShader* getShadowlessShader() { return shadowlessShader.get(); }

	void render();

	void renderWithoutShadows();

	void cleanup();

	

};


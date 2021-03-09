#pragma once
#include "utils/Utils.h"
#include "AdvancedShader.h"
#include "Model.h"
#include "ICamera.h"
#include "utils/Settings.h"
#include "Light.h"
/*!
* Renders every entity that is visible in the level, meaning terrain, trees, campfires, player-characters,...
*/
class LevelRenderer
{
private:

	std::shared_ptr<AdvancedShader> shader;
	std::shared_ptr<AdvancedShader> shadowlessShader;
	std::vector<Model*>  modelList;

public:
	LevelRenderer();

	/*!
	* sets the uniforms needed for the corresponding shaders
	*/
	void setUniforms(bool shadows, ICamera* camera, glm::vec4 clippingPlane, bool lightMapping, bool normalMapping, std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights, GLuint depthCubemap = 0);

	/*!
	* adds a Model to the renderlist of this level renderer
	*/
	void addModel(string path, glm::vec3 position, glm::vec3 scale);
	void addModel(Model* model);
	void addModels(std::vector<Model*> models);

	std::vector<Model*>* getModels() { return &modelList; }
	AdvancedShader* getShader() { return shader.get(); }
	AdvancedShader* getShadowlessShader() { return shadowlessShader.get(); }

	/*!
	* renders the scene using the shader that incorporates omnidirectional shadowmapping
	*/
	void render();

	/*!
	* renders the scene using the shader without shadows
	*/
	void renderWithoutShadows();

	/*!
	* deletes the shader programs and cleans up everything openGL related
	*/
	void cleanup();
};

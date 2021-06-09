#pragma once
#include "utils/Utils.h"
#include "AdvancedShader.h"
#include "AnimatedModel.h"
#include "BasicCamera.h"
#include "Light.h"
class AnimatedModelRenderer
{
private:
	std::shared_ptr<AdvancedShader> shader;

	const int MAX_JOINTS = 50;

public:

	AnimatedModelRenderer();


	void render(AnimatedModel* entity, ICamera* camera);

	void prepareRender(ICamera* camera, glm::vec4 clippingPlane, bool lightMapping, bool normalMapping, std::vector<DirectionalLight*> dirLights, std::vector<PointLight*> pointLights);

	void finish();
};


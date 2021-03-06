#pragma once
#include "utils/Utils.h"
#include "utils/Settings.h"
#include "AdvancedShader.h"
#include "OmniShadowFrameBuffer.h"
#include "BasicCamera.h"
#include "Light.h"
class OmniShadowRenderer
{
private:
	std::shared_ptr<AdvancedShader> shader;

	/*	GLuint locShadowMatrices;
		uniform vec3 lightPos;
		uniform float farPlane;
	*/
public:
	OmniShadowRenderer(string vf, string gf, string ff);
	OmniShadowRenderer();
	void prepareRender(PointLight* pointLight);
	void cleanUpAfterRender(PointLight* pointLight);
	void cleanup();
	std::shared_ptr<AdvancedShader> getShader() { return shader; }

};


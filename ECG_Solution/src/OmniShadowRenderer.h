#pragma once
#include "utils/Utils.h"
#include "utils/Settings.h"
#include "AdvancedShader.h"
#include "OmniShadowFrameBuffer.h"
#include "BasicCamera.h"
#include "Light.h"

/*!
* Manages the rendering process and the shader for creating the shadow maps
*/
class OmniShadowRenderer
{
private:
	std::shared_ptr<AdvancedShader> shader;
public:
	OmniShadowRenderer(string vf, string gf, string ff);
	OmniShadowRenderer();

	/*!
	* sets up the game engine to render the shadow map
	* MUST be called before rendering the level
	*/
	void prepareRender(PointLight* pointLight);
	/*!
	* cleans up after rendering the shadow map
	* MUST be called after rendering the level
	*/
	void cleanUpAfterRender(PointLight* pointLight);
	void cleanup();
	std::shared_ptr<AdvancedShader> getShader() { return shader; }
};

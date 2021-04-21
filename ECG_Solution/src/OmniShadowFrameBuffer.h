#pragma once
#include "IFBO.h"
#include "utils/Utils.h"
#include "utils/Settings.h"
/*!
* Manages the Framebuffers needed for creating the Shadows of one Lightsource
*/
class OmniShadowFrameBuffer :
	public IFBO
{
private:
	//The Cubemap texture
	GLuint depthMap;
	//the FBO in which the shadow map will be rendered to
	GLuint depthFBO;

	void bindFBO(GLuint fbo);
public:
	OmniShadowFrameBuffer();

	void bindFBO();
	void unbindFBO();

	void shutdown();

	GLuint getDepthMap() { return depthMap; }
	GLuint getDepthFBO() { return depthFBO; }
};

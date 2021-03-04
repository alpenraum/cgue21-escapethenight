#pragma once
#include "utils/Utils.h"
#include "IFBO.h"
class WaterFrameBuffer : public IFBO
{
private:
	GLuint _reflectionFBO;
	GLuint _reflectionTexture;
	GLuint _reflectionDepthBuffer;

	GLuint _refractionFBO;
	GLuint _refractionTexture;
	GLuint _refractionDepthTexture;

	void initReflectionFBO(int width, int height);
	void initRefractionFBO(int width, int height);

	void bindFBO(GLuint fbo);

public:
	WaterFrameBuffer();
	WaterFrameBuffer(int width, int height);


	void bindReflectionFBO();
	void bindRefractionFBO();
	void unbindFBO();

	GLuint getReflectionFBO() const { return _reflectionFBO; }
	void SetReflectionFBO(GLuint val) { _reflectionFBO = val; }
	GLuint getReflectionTexture() const { return _reflectionTexture; }
	void SetReflectionTexture(GLuint val) { _reflectionTexture = val; }
	GLuint getReflectionDepthBuffer() const { return _reflectionDepthBuffer; }
	void SetReflectionDepthBuffer(GLuint val) { _reflectionDepthBuffer = val; }
	GLuint getRefractionFBO() const { return _refractionFBO; }
	void setRefractionFBO(GLuint val) { _refractionFBO = val; }
	GLuint getRefractionTexture() const { return _refractionTexture; }
	void SetRefractionTexture(GLuint val) { _refractionTexture = val; }
	GLuint getRefractionDepthTexture() const { return _refractionDepthTexture; }
	void setRefractionDepthTexture(GLuint val) { _refractionDepthTexture = val; }

	void shutdown();
};


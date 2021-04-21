#include "WaterFrameBuffer.h"

WaterFrameBuffer::WaterFrameBuffer() {}
WaterFrameBuffer::WaterFrameBuffer(int width, int height) {
	initRefractionFBO(width, height);
	initReflectionFBO(width, height);
}
void WaterFrameBuffer::bindReflectionFBO()
{
	bindFBO(_reflectionFBO);
}
void WaterFrameBuffer::bindRefractionFBO()
{
	bindFBO(_refractionFBO);
}
void WaterFrameBuffer::unbindFBO()
{
	bindFBO(0);
}
void WaterFrameBuffer::shutdown()
{
	glDeleteFramebuffers(1, &_reflectionFBO);
	glDeleteTextures(1, &_reflectionTexture);
	glDeleteRenderbuffers(1, &_reflectionDepthBuffer);
	glDeleteFramebuffers(1, &_refractionFBO);
	glDeleteTextures(1, &_refractionTexture);
	glDeleteTextures(1, &_refractionDepthTexture);
}
void WaterFrameBuffer::initReflectionFBO(int width, int height)
{
	glGenFramebuffers(1, &_reflectionFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _reflectionFBO);

	glGenTextures(1, &_reflectionTexture);
	glBindTexture(GL_TEXTURE_2D, _reflectionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _reflectionTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &_reflectionDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _reflectionDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _reflectionDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Reflection_Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WaterFrameBuffer::initRefractionFBO(int width, int height)
{
	glGenFramebuffers(1, &_refractionFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _refractionFBO);

	glGenTextures(1, &_refractionTexture);
	glBindTexture(GL_TEXTURE_2D, _refractionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _refractionTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &_refractionDepthTexture);
	glBindTexture(GL_TEXTURE_2D, _refractionDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _refractionDepthTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Refraction_Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void WaterFrameBuffer::bindFBO(GLuint fbo)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}
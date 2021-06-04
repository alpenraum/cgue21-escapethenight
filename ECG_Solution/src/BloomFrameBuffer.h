#pragma once

#include"utils/Utils.h"
#include "utils/Settings.h"

class BloomFrameBuffer
{
private:
	
	GLuint pingPongFBO[2];
	GLuint pingPongColorBuffers[2];

public:

	BloomFrameBuffer();

	void bindPingPongFBO(int index);

	void unbindFBO();

	GLuint getPingPongColorBuffer(int index);
	
};


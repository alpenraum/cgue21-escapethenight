#pragma once
#include "utils/Utils.h"
class IFBO {
public:
	virtual void bindFBO(GLuint fbo) = 0;
	virtual void shutdown() = 0;
};

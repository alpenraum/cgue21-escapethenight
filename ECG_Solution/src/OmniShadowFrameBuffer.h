#pragma once
#include "IFBO.h"
#include "utils/Utils.h"
#include "utils/Settings.h"
class OmniShadowFrameBuffer :
    public IFBO
{
private:
    GLuint depthMap;
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


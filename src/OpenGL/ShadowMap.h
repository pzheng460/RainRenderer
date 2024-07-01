#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <utils/shader.h>
#include <utils/filesystem.h>

class ShadowMap
{
public:
    ShadowMap();
    void init();
    void preDraw();
    void postDraw();
    unsigned int getDepthMap() const { return depthMap; }

private:
    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
    unsigned int depthMapFBO;
    unsigned int depthMap;
};

#endif // SHADOWMAP_H

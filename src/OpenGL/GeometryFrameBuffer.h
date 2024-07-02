#ifndef GEOMETRYFRAMEBUFFER_H
#define GEOMETRYFRAMEBUFFER_H

#include "FrameBuffer.h"

class GeometryPositionColorTexture final : public ColorTexture {
public:
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class GeometryNormalColorTexture final : public ColorTexture {
public:
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class GeometryAlbedoSpecColorTexture final : public ColorTexture {
public:
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class GeometryFrameBuffer : public FrameBuffer {
public:
    GeometryFrameBuffer(int numOfColorTextureAttachments = 3, int numOfDepthTextureAttachments = 0, int numOfRenderBufferObjectDepth = 0);
};
#endif // GEOMETRYFRAMEBUFFER_H

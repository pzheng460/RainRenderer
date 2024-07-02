#ifndef GEOMETRYFRAMEBUFFER_H
#define GEOMETRYFRAMEBUFFER_H

#include "FrameBuffer.h"

class GeometryPositionTexture final : public Texture {
public:
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class GeometryNormalTexture final : public Texture {
public:
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class GeometryAlbedoSpecTexture final : public Texture {
public:
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class GeometryFrameBuffer : public FrameBuffer {
public:
    GeometryFrameBuffer(int numOfColorAttachments = 1, int numOfDepthAttachments = 0);
};
#endif // GEOMETRYFRAMEBUFFER_H

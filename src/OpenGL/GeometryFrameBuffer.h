#ifndef GEOMETRYFRAMEBUFFER_H
#define GEOMETRYFRAMEBUFFER_H

#include "FrameBuffer.h"

class GeometryPositionColorTexture final : public Texture {
public:
    GeometryPositionColorTexture() {
        internalFormat = GL_RGBA16F;
        format = GL_RGBA;
        type = GL_FLOAT;
    }
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT, GLvoid* data) override;
};

class GeometryNormalColorTexture final : public Texture {
public:
    GeometryNormalColorTexture() {
        internalFormat = GL_RGBA16F;
        format = GL_RGBA;
        type = GL_FLOAT;
    }
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT, GLvoid* data) override;
};

class GeometryAlbedoSpecColorTexture final : public Texture {
public:
    GeometryAlbedoSpecColorTexture() {
        internalFormat = GL_RGBA;
        format = GL_RGBA;
        type = GL_UNSIGNED_BYTE;
    }
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT, GLvoid* data) override;
};

class GeometryFrameBuffer : public FrameBuffer {
public:
    GeometryFrameBuffer(int numOfColorTextureAttachments = 3, int numOfDepthTextureAttachments = 0, int numOfRenderBufferObjectDepth = 0);
};
#endif // GEOMETRYFRAMEBUFFER_H

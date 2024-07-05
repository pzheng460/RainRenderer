#ifndef GEOMETRYFRAMEBUFFER_H
#define GEOMETRYFRAMEBUFFER_H

#include "FrameBuffer.h"

class GeometryPositionColorTexture final : public Texture {
public:
    GeometryPositionColorTexture() {
        target = GL_TEXTURE_2D;
        internalFormat = GL_RGBA16F;
        format = GL_RGBA;
        type = GL_FLOAT;

        minFilter = GL_NEAREST;
        magFilter = GL_NEAREST;
        wrapS = GL_CLAMP_TO_EDGE;
        wrapT = GL_CLAMP_TO_EDGE;
    }
    void specifyTexture(GLvoid* data) override;
};

class GeometryNormalColorTexture final : public Texture {
public:
    GeometryNormalColorTexture() {
        target = GL_TEXTURE_2D;
        internalFormat = GL_RGBA16F;
        format = GL_RGBA;
        type = GL_FLOAT;
    }
    void specifyTexture(GLvoid* data) override;
};

class GeometryAlbedoSpecColorTexture final : public Texture {
public:
    GeometryAlbedoSpecColorTexture() {
        target = GL_TEXTURE_2D;
        internalFormat = GL_RGBA;
        format = GL_RGBA;
        type = GL_UNSIGNED_BYTE;
    }
    void specifyTexture(GLvoid* data) override;
};

class GeometryFrameBuffer : public FrameBuffer {
public:
    GeometryFrameBuffer(int numOfColorTextureAttachments = 3, int numOfDepthTextureAttachments = 0, int numOfRenderBufferObjectDepth = 0);
};
#endif // GEOMETRYFRAMEBUFFER_H

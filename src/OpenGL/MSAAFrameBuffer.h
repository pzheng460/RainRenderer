#ifndef MSAAFRAMEBUFFER_H
#define MSAAFRAMEBUFFER_H

#include "FrameBuffer.h"

class MSAAColorTexture final : public Texture {
public:
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT, GLvoid* data) override;
private:
    int samples = 4;
};

class MSAARenderBufferObjectDepth final : public RenderBufferObject {
public:
    void generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT) override;
private:
    int samples = 4;
};

class MSAAFrameBuffer final : public FrameBuffer {
public:
    MSAAFrameBuffer(int numOfColorTextureAttachments = 1, int numOfDepthTextureAttachments = 0, int numOfRenderBufferObjectDepth = 1);
    void bindColorTextureAttachment() override;
    void bindRenderBufferDepthAttachment() override;
    void generateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT) override;
};

#endif // MSAAFRAMEBUFFER_H

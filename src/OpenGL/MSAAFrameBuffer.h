#ifndef MSAAFRAMEBUFFER_H
#define MSAAFRAMEBUFFER_H

#include "FrameBuffer.h"

class MSAATexture final : public Texture {
public:
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) override;
private:
    int samples = 4;
};

class MSAARenderBufferObjectDepth final : public RenderBufferObjectDepth {
public:
    void generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT) override;
private:
    int samples = 4;
};

class MSAAFrameBuffer final : public FrameBuffer {
public:
    MSAAFrameBuffer(int numOfColorAttachments = 1, int numOfDepthAttachments = 0);
    void bindColorTextureAttachment() override;
    void bindRenderBufferDepthAttachment() override;
    void generateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT) override;
};

#endif // MSAAFRAMEBUFFER_H

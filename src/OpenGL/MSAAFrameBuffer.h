#ifndef MSAAFRAMEBUFFER_H
#define MSAAFRAMEBUFFER_H

#include "FrameBuffer.h"

class MSAAFrameBuffer : public FrameBuffer {
public:
    MSAAFrameBuffer(int numOfColorAttachments = 1, int numOfDepthAttachments = 0);
    void createColorTextureAttachment(int SCR_WIDTH, int SCR_HEIGHT) override;
    void createRenderBufferAttachment(int SCR_WIDTH, int SCR_HEIGHT) override;
};

#endif // MSAAFRAMEBUFFER_H

#ifndef MSAAFRAMEBUFFER_H
#define MSAAFRAMEBUFFER_H

#include "FrameBuffer.h"

class MSAAFrameBuffer final : public FrameBuffer {
public:
    MSAAFrameBuffer(int numOfColorTextureAttachments = 1, int numOfDepthTextureAttachments = 0, int numOfRenderBufferObjectDepth = 1);
};

#endif // MSAAFRAMEBUFFER_H

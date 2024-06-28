#ifndef PINGPONGFRAMEBUFFER_H
#define PINGPONGFRAMEBUFFER_H

#include "FrameBuffer.h"

class PingPongFrameBuffer : public FrameBuffer {
public:
    PingPongFrameBuffer(int numOfColorAttachments = 1, int numOfDepthAttachments = 0);
    void createColorTextureAttachment(int SCR_WIDTH, int SCR_HEIGHT) override;
};

#endif // PINGPONGFRAMEBUFFER_H

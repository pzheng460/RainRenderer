#ifndef GEOMETRYFRAMEBUFFER_H
#define GEOMETRYFRAMEBUFFER_H

#include "FrameBuffer.h"

class GeometryFrameBuffer : public FrameBuffer {
public:
    GeometryFrameBuffer(int numOfColorTextureAttachments = 3, int numOfDepthTextureAttachments = 0, int numOfRenderBufferObjectDepth = 0);
};
#endif // GEOMETRYFRAMEBUFFER_H

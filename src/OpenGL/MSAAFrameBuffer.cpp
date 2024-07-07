#include "MSAAFrameBuffer.h"

MSAAFrameBuffer::MSAAFrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth) :
    FrameBuffer(numOfColorTextureAttachments, numOfDepthTextureAttachments, numOfRenderBufferObjectDepth) {
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        auto textureColorBufferPtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_MSAA_COLOR_ATTACHMENT);
        auto textureColorBuffer = std::shared_ptr<Texture>(textureColorBufferPtr);
        textureColorBuffers[i] = std::move(textureColorBuffer);
    }
    rboDepth = std::make_shared<RenderBufferObjectDepthMultiSample>();
}


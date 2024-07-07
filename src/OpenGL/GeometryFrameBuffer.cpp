#include "GeometryFrameBuffer.h"

GeometryFrameBuffer::GeometryFrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth) :
    FrameBuffer(numOfColorTextureAttachments, numOfDepthTextureAttachments, numOfRenderBufferObjectDepth) {
    auto texturePositionColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_GEOMETRY_POSITION_COLOR_ATTACHMENT);
    textureColorBuffers[0] = std::shared_ptr<Texture>(texturePositionColorBuffer);
    auto textureNormalColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_GEOMETRY_NORMAL_COLOR_ATTACHMENT);
    textureColorBuffers[1] = std::shared_ptr<Texture>(textureNormalColorBuffer);
    auto textureAlbedoSpecColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_GEOMETRY_ALBEDO_SPEC_COLOR_ATTACHMENT);
    textureColorBuffers[2] = std::shared_ptr<Texture>(textureAlbedoSpecColorBuffer);
}
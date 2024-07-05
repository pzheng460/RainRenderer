#include "GeometryFrameBuffer.h"

void GeometryPositionColorTexture::specifyTexture(GLvoid *data) {
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
}

void GeometryNormalColorTexture::specifyTexture(GLvoid *data) {
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
}

void GeometryAlbedoSpecColorTexture::specifyTexture(GLvoid *data) {
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
}

GeometryFrameBuffer::GeometryFrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth) :
    FrameBuffer(numOfColorTextureAttachments, numOfDepthTextureAttachments, numOfRenderBufferObjectDepth) {
    auto texturePositionColorBuffer = std::make_shared<GeometryPositionColorTexture>();
    textureColorBuffers[0] = std::move(texturePositionColorBuffer);
    auto textureNormalColorBuffer = std::make_shared<GeometryNormalColorTexture>();
    textureColorBuffers[1] = std::move(textureNormalColorBuffer);
    auto textureAlbedoSpecColorBuffer = std::make_shared<GeometryAlbedoSpecColorTexture>();
    textureColorBuffers[2] = std::move(textureAlbedoSpecColorBuffer);
}
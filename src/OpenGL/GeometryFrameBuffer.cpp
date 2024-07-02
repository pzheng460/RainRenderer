#include "GeometryFrameBuffer.h"

void GeometryPositionColorTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GeometryNormalColorTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GeometryAlbedoSpecColorTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GeometryFrameBuffer::GeometryFrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth) :
    FrameBuffer(numOfColorTextureAttachments, numOfDepthTextureAttachments, numOfRenderBufferObjectDepth) {
    auto texturePositionColorBuffer = std::make_unique<GeometryPositionColorTexture>();
    textureColorBuffers[0] = std::move(texturePositionColorBuffer);
    auto textureNormalColorBuffer = std::make_unique<GeometryNormalColorTexture>();
    textureColorBuffers[1] = std::move(textureNormalColorBuffer);
    auto textureAlbedoSpecColorBuffer = std::make_unique<GeometryAlbedoSpecColorTexture>();
    textureColorBuffers[2] = std::move(textureAlbedoSpecColorBuffer);
}
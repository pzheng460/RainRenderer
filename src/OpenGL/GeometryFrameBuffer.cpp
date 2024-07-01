#include "GeometryFrameBuffer.h"

void GeometryPositionTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GeometryNormalTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GeometryAlbedoSpecTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GeometryFrameBuffer::GeometryFrameBuffer(int numOfColorAttachments, int numOfDepthAttachments)
        : FrameBuffer(numOfColorAttachments, numOfDepthAttachments) {
    std::unique_ptr<GeometryPositionTexture> texturePositionColorBuffer = std::make_unique<GeometryPositionTexture>();
    textureColorBuffers[0] = std::move(texturePositionColorBuffer);
    std::unique_ptr<GeometryNormalTexture> textureNormalColorBuffer = std::make_unique<GeometryNormalTexture>();
    textureColorBuffers[1] = std::move(textureNormalColorBuffer);
    std::unique_ptr<GeometryAlbedoSpecTexture> textureAlbedoSpecColorBuffer = std::make_unique<GeometryAlbedoSpecTexture>();
    textureColorBuffers[2] = std::move(textureAlbedoSpecColorBuffer);
}
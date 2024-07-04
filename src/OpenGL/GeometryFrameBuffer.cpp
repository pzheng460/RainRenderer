#include "GeometryFrameBuffer.h"

void GeometryPositionColorTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT, GLvoid* data) {
    this->width = SCR_WIDTH;
    this->height = SCR_HEIGHT;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GeometryNormalColorTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT, GLvoid* data) {
    this->width = SCR_WIDTH;
    this->height = SCR_HEIGHT;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GeometryAlbedoSpecColorTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT, GLvoid* data) {
    this->width = SCR_WIDTH;
    this->height = SCR_HEIGHT;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
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
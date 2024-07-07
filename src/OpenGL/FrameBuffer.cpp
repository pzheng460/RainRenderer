#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth) :
    numOfColorTextureAttachments(numOfColorTextureAttachments), numOfDepthTextureAttachments(numOfDepthTextureAttachments), numOfRenderBufferObjectDepth(numOfRenderBufferObjectDepth) {
    glGenFramebuffers(1, &framebuffer);
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        auto textureColorBufferPtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_COMMON_COLOR_ATTACHMENT);
        textureColorBuffers.push_back(std::shared_ptr<Texture>(textureColorBufferPtr));
    }
    if (numOfDepthTextureAttachments > 0) {
        for (int i = 0; i < numOfDepthTextureAttachments; i++) {
            auto textureDepthBufferPtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_COMMON_DEPTH_ATTACHMENT);
            textureDepthBuffer = std::shared_ptr<Texture>(textureDepthBufferPtr);
        }
    }
    if (numOfRenderBufferObjectDepth > 0) {
        rboDepth = std::make_shared<RenderBufferObjectDepth>();
    }
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindColorTextureAttachment() {
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textureColorBuffers[i]->target, textureColorBuffers[i]->textureID, 0);
    }
}

void FrameBuffer::bindDepthTextureAttachment() {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureDepthBuffer->target, textureDepthBuffer->textureID, 0);
}

void FrameBuffer::bindRenderBufferDepthAttachment() {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth->rbo);
}

bool FrameBuffer::checkComplete() {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER::" << toString(frameBufferFactoryType) << " is not complete!" << std::endl;
        return false;
    }
    return true;
}

void FrameBuffer::generateFrameBuffer(int newWidth, int newHeight) {
    setSize(newWidth, newHeight);
    bind();

    // create floating point color buffer 创建浮点颜色缓冲区
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        textureColorBuffers[i]->generateTexture(width, height, nullptr);
    }
    bindColorTextureAttachment();

    // create depth texture 创建深度纹理
    if (numOfDepthTextureAttachments > 0) {
        textureDepthBuffer->generateTexture(this->width, this->height, nullptr);
        bindDepthTextureAttachment();
    }

    // create depth buffer (renderbuffer) 创建深度缓冲区（渲染缓冲区）
    if (numOfRenderBufferObjectDepth > 0) {
        rboDepth->generateRenderBufferObject(this->width, this->height);
        bindRenderBufferDepthAttachment();
    }

    if (numOfColorTextureAttachments > 0) {
        // 动态设置绘制缓冲区
        std::vector<GLuint> attachments(numOfColorTextureAttachments);
        for (int i = 0; i < numOfColorTextureAttachments; ++i) {
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glDrawBuffers(numOfColorTextureAttachments, attachments.data());
    } else {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    // check if frame buffer is complete 检查帧缓冲是否完整
    checkComplete();
    unbind();
}

void FrameBuffer::transferFrameBuffer(FrameBuffer& targetFrameBuffer) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFrameBuffer.framebuffer);
    glBlitFramebuffer(0, 0, width, height, 0, 0, targetFrameBuffer.width, targetFrameBuffer.height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType frameBufferFactoryType) {
    FrameBuffer frameBuffer;
    if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_SHADOW_MAP)
    {
        frameBuffer = FrameBuffer(0, 1, 0);
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_MSAA)
    {
        frameBuffer = FrameBuffer(2, 0, 1);
        for (int i = 0; i < frameBuffer.numOfColorTextureAttachments; i++) {
            auto textureColorBufferPtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_MSAA_COLOR_ATTACHMENT);
            auto textureColorBuffer = std::shared_ptr<Texture>(textureColorBufferPtr);
            frameBuffer.textureColorBuffers[i] = std::move(textureColorBuffer);
        }
        frameBuffer.rboDepth = std::make_shared<RenderBufferObjectDepthMultiSample>();
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_INTERMEDIATE)
    {
        frameBuffer = FrameBuffer(2, 0, 0);
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_PING_PONG)
    {
        frameBuffer = FrameBuffer(1, 0, 0);
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_GEOMETRY)
    {
        frameBuffer = FrameBuffer(3, 0, 1);
        auto texturePositionColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_GEOMETRY_POSITION_COLOR_ATTACHMENT);
        frameBuffer.textureColorBuffers[0] = std::shared_ptr<Texture>(texturePositionColorBuffer);
        auto textureNormalColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_GEOMETRY_NORMAL_COLOR_ATTACHMENT);
        frameBuffer.textureColorBuffers[1] = std::shared_ptr<Texture>(textureNormalColorBuffer);
        auto textureAlbedoSpecColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_GEOMETRY_ALBEDO_SPEC_COLOR_ATTACHMENT);
        frameBuffer.textureColorBuffers[2] = std::shared_ptr<Texture>(textureAlbedoSpecColorBuffer);
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_SSAO)
    {
        frameBuffer = FrameBuffer(1, 0, 0);
        for (int i = 0; i < frameBuffer.numOfColorTextureAttachments; i++) {
            auto textureColorBufferPtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_SSAO_COLOR_ATTACHMENT);
            auto textureColorBuffer = std::shared_ptr<Texture>(textureColorBufferPtr);
            frameBuffer.textureColorBuffers[i] = std::move(textureColorBuffer);
        }
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_SSAO_BLUR)
    {
        frameBuffer = FrameBuffer(1, 0, 0);
        for (int i = 0; i < frameBuffer.numOfColorTextureAttachments; i++) {
            auto textureColorBufferPtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_SSAO_COLOR_ATTACHMENT);
            auto textureColorBuffer = std::shared_ptr<Texture>(textureColorBufferPtr);
            frameBuffer.textureColorBuffers[i] = std::move(textureColorBuffer);
        }
    }
    else
    {
        std::cerr << "FrameBufferFactoryType not found" << std::endl;
    }
    frameBuffer.frameBufferFactoryType = frameBufferFactoryType;
    return frameBuffer;
}
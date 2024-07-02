#include "MSAAFrameBuffer.h"

void MSAAColorTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void MSAARenderBufferObjectDepth::generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT) {
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

MSAAFrameBuffer::MSAAFrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth) :
    FrameBuffer(numOfColorTextureAttachments, numOfDepthTextureAttachments, numOfRenderBufferObjectDepth) {
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        auto textureColorBuffer = std::make_unique<MSAAColorTexture>();
        textureColorBuffers[i] = std::move(textureColorBuffer);
    }
    rboDepth = std::make_unique<MSAARenderBufferObjectDepth>();
}

void MSAAFrameBuffer::bindColorTextureAttachment() {
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffers[i]->getTexture(), 0);
    }
}

void MSAAFrameBuffer::bindRenderBufferDepthAttachment() {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepth->getRBO());
}

void MSAAFrameBuffer::generateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT) {
    bind();

    // create floating point color buffer 创建浮点颜色缓冲区
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        textureColorBuffers[i]->generateTexture(SCR_WIDTH, SCR_HEIGHT);
    }
    bindColorTextureAttachment();

    // create depth buffer (renderbuffer) 创建深度缓冲区（渲染缓冲区）
    if (numOfRenderBufferObjectDepth > 0) {
        rboDepth->generateRenderBufferObject(SCR_WIDTH, SCR_HEIGHT);
        bindRenderBufferDepthAttachment();
    }

    // 动态设置绘制缓冲区
    std::vector<GLuint> attachments(numOfColorTextureAttachments);
    for (int i = 0; i < numOfColorTextureAttachments; ++i) {
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(numOfColorTextureAttachments, attachments.data());

    // check if frame buffer is complete 检查帧缓冲是否完整
    checkComplete();
    unbind();
}

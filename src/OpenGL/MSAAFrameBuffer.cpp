#include "MSAAFrameBuffer.h"

void MSAATexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
}

void MSAARenderBufferObjectDepth::generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT) {
    RenderBufferObject::generateRenderBufferObject(SCR_WIDTH, SCR_HEIGHT);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

MSAAFrameBuffer::MSAAFrameBuffer(int numOfColorAttachments, int numOfDepthAttachments) : FrameBuffer(numOfColorAttachments, numOfDepthAttachments) {
    msaaTextureColorBuffers.resize(numOfColorAttachments);
}

void MSAAFrameBuffer::bindColorTextureAttachment() {
    for (int i = 0; i < numOfColorAttachments; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, msaaTextureColorBuffers[i].getTexture(), 0);
    }
    // 动态设置绘制缓冲区
    std::vector<GLuint> attachments(numOfColorAttachments);
    for (int i = 0; i < numOfColorAttachments; ++i) {
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(numOfColorAttachments, attachments.data());
}

void MSAAFrameBuffer::bindRenderBufferDepthAttachment() {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msaaRBODepth.getRBO());
}

void MSAAFrameBuffer::generateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT) {
    init();
    bind();

    // create floating point color buffer 创建浮点颜色缓冲区
    for (int i = 0; i < numOfColorAttachments; i++) {
        msaaTextureColorBuffers[i].generateTexture(SCR_WIDTH, SCR_HEIGHT);
    }
    bindColorTextureAttachment();

    // create depth buffer (renderbuffer) 创建深度缓冲区（渲染缓冲区）
    if (numOfDepthAttachments > 0) {
        msaaRBODepth.generateRenderBufferObject(SCR_WIDTH, SCR_HEIGHT);
        bindRenderBufferDepthAttachment();
    }

    // check if frame buffer is complete 检查帧缓冲是否完整
    checkComplete();
    unbind();
}

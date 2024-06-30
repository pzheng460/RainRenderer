#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer(int numOfColorAttachments, int numOfDepthAttachments) : numOfColorAttachments(numOfColorAttachments), numOfDepthAttachments(numOfDepthAttachments) {
    textureColorBuffers.resize(numOfColorAttachments);
}

void FrameBuffer::init() {
    glGenFramebuffers(1, &framebuffer);
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::createColorTextureAttachment(int SCR_WIDTH, int SCR_HEIGHT) {
    for (int i = 0; i < numOfColorAttachments; i++) {
        glGenTextures(1, &textureColorBuffers[i]);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureColorBuffers[i], 0);
    }
    // 动态设置绘制缓冲区
    std::vector<GLuint> attachments(numOfColorAttachments);
    for (int i = 0; i < numOfColorAttachments; ++i) {
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(numOfColorAttachments, attachments.data());
}

void FrameBuffer::createRenderBufferAttachment(int SCR_WIDTH, int SCR_HEIGHT) {
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

bool FrameBuffer::checkComplete() {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        return false;
    }
    return true;
}

void FrameBuffer::generateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT) {
    init();
    bind();
    // create floating point color buffer 创建浮点颜色缓冲区
    createColorTextureAttachment(SCR_WIDTH, SCR_HEIGHT);
    // create depth buffer (renderbuffer) 创建深度缓冲区（渲染缓冲区）
    if (numOfDepthAttachments > 0)
        createRenderBufferAttachment(SCR_WIDTH, SCR_HEIGHT);
    // check if frame buffer is complete 检查帧缓冲是否完整
    checkComplete();
    unbind();
}

void FrameBuffer::transferFrameBuffer(FrameBuffer& targetFrameBuffer, int SCR_WIDTH, int SCR_HEIGHT) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFrameBuffer.framebuffer);
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
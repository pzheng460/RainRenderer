#include "FrameBuffer.h"
#include <iostream>

void Textures::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderBufferObject::generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT) {
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
}

void RenderBufferObjectDepth::generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT) {
    RenderBufferObject::generateRenderBufferObject(SCR_WIDTH, SCR_HEIGHT);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

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

void FrameBuffer::bindColorTextureAttachment() {
    for (int i = 0; i < numOfColorAttachments; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureColorBuffers[i].getTexture(), 0);
    }
    // 动态设置绘制缓冲区
    std::vector<GLuint> attachments(numOfColorAttachments);
    for (int i = 0; i < numOfColorAttachments; ++i) {
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(numOfColorAttachments, attachments.data());
}

void FrameBuffer::bindRenderBufferDepthAttachment() {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth.getRBO());
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
    for (int i = 0; i < numOfColorAttachments; i++) {
        textureColorBuffers[i].generateTexture(SCR_WIDTH, SCR_HEIGHT);
    }
    bindColorTextureAttachment();

    // create depth buffer (renderbuffer) 创建深度缓冲区（渲染缓冲区）
    if (numOfDepthAttachments > 0) {
        rboDepth.generateRenderBufferObject(SCR_WIDTH, SCR_HEIGHT);
        bindRenderBufferDepthAttachment();
    }

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
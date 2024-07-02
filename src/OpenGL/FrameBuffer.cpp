#include "FrameBuffer.h"
#include <iostream>

void ColorTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void DepthTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderBufferObjectDepth::generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT) {
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

FrameBuffer::FrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth) :
    numOfColorTextureAttachments(numOfColorTextureAttachments), numOfDepthTextureAttachments(numOfDepthTextureAttachments), numOfRenderBufferObjectDepth(numOfRenderBufferObjectDepth) {
    init();
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        auto textureColorBuffer = std::make_unique<ColorTexture>();
        textureColorBuffers.push_back(std::move(textureColorBuffer));
    }
    if (numOfDepthTextureAttachments > 0) {
        for (int i = 0; i < numOfDepthTextureAttachments; i++) {
            textureDepthBuffer = std::make_unique<DepthTexture>();
        }
    }
    if (numOfRenderBufferObjectDepth > 0) {
        rboDepth = std::make_unique<RenderBufferObjectDepth>();
    }
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
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureColorBuffers[i]->getTexture(), 0);
    }
}

void FrameBuffer::bindDepthTextureAttachment() {
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureDepthBuffer->getTexture(), 0);
}

void FrameBuffer::bindRenderBufferDepthAttachment() {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth->getRBO());
}

bool FrameBuffer::checkComplete() {
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        return false;
    }
    return true;
}

void FrameBuffer::generateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT) {
    bind();

    // create floating point color buffer 创建浮点颜色缓冲区
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        textureColorBuffers[i]->generateTexture(SCR_WIDTH, SCR_HEIGHT);
    }
    bindColorTextureAttachment();

    // create depth texture 创建深度纹理
    if (numOfDepthTextureAttachments > 0) {
        textureDepthBuffer->generateTexture(SCR_WIDTH, SCR_HEIGHT);
        bindDepthTextureAttachment();
    }

    // create depth buffer (renderbuffer) 创建深度缓冲区（渲染缓冲区）
    if (numOfRenderBufferObjectDepth > 0) {
        rboDepth->generateRenderBufferObject(SCR_WIDTH, SCR_HEIGHT);
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

void FrameBuffer::transferFrameBuffer(FrameBuffer& targetFrameBuffer, int SCR_WIDTH, int SCR_HEIGHT) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targetFrameBuffer.framebuffer);
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
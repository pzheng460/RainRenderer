#include "FrameBuffer.h"
#include <iostream>
#include <cxxabi.h>

void ColorTexture::specifyTexture(GLvoid *data) {
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
}

void DepthTexture::specifyTexture(GLvoid *data) {
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
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
        auto textureColorBuffer = std::make_shared<ColorTexture>();
        textureColorBuffers.push_back(std::move(textureColorBuffer));
    }
    if (numOfDepthTextureAttachments > 0) {
        for (int i = 0; i < numOfDepthTextureAttachments; i++) {
            textureDepthBuffer = std::make_shared<DepthTexture>();
        }
    }
    if (numOfRenderBufferObjectDepth > 0) {
        rboDepth = std::make_shared<RenderBufferObjectDepth>();
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
        int status;
        char* demangledName = abi::__cxa_demangle(typeid(*this).name(), nullptr, nullptr, &status);
        std::string typeName = (status == 0) ? demangledName : typeid(*this).name();
        std::cout << "ERROR::FRAMEBUFFER::" << typeName << " is not complete!" << std::endl;
        free(demangledName); // free memory
        return false;
    }
    return true;
}

void FrameBuffer::generateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT) {
    bind();

    // create floating point color buffer 创建浮点颜色缓冲区
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        textureColorBuffers[i]->generateTexture(SCR_WIDTH, SCR_HEIGHT, nullptr);
    }
    bindColorTextureAttachment();

    // create depth texture 创建深度纹理
    if (numOfDepthTextureAttachments > 0) {
        textureDepthBuffer->generateTexture(SCR_WIDTH, SCR_HEIGHT, nullptr);
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
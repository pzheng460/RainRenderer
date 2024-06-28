#include "MSAAFrameBuffer.h"

MSAAFrameBuffer::MSAAFrameBuffer(int numOfColorAttachments, int numOfDepthAttachments) : FrameBuffer(numOfColorAttachments, numOfDepthAttachments) {}

void MSAAFrameBuffer::createColorTextureAttachment(int SCR_WIDTH, int SCR_HEIGHT) {
    for (int i = 0; i < numOfColorAttachments; i++) {
        glGenTextures(1, &textureColorBuffers[i]);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffers[i]);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D_MULTISAMPLE, textureColorBuffers[i], 0);
    }
    // 动态设置绘制缓冲区
    std::vector<GLuint> attachments(numOfColorAttachments);
    for (int i = 0; i < numOfColorAttachments; ++i) {
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }
    glDrawBuffers(numOfColorAttachments, attachments.data());
}

void MSAAFrameBuffer::createRenderBufferAttachment(int SCR_WIDTH, int SCR_HEIGHT) {
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

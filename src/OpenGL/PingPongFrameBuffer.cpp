#include "PingPongFrameBuffer.h"

PingPongFrameBuffer::PingPongFrameBuffer(int numOfColorAttachments, int numOfDepthAttachments) : FrameBuffer(numOfColorAttachments, numOfDepthAttachments) {}

void PingPongFrameBuffer::createColorTextureAttachment(int SCR_WIDTH, int SCR_HEIGHT) {
    for (int i = 0; i < numOfColorAttachments; i++) {
        glGenTextures(1, &textureColorBuffers[i]);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffers[i]);
        // * 2 for the retina display of Mac
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
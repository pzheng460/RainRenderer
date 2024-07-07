#include "RenderBufferObject.h"

void RenderBufferObjectDepth::generateRenderBufferObject(int newWidth, int newHeight) {
    setSize(newWidth, newHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBufferObjectDepthMultiSample::generateRenderBufferObject(int newWidth, int newHeight) {
    setSize(newWidth, newHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
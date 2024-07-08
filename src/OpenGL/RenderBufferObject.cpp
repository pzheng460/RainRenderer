#include "RenderBufferObject.h"

void RenderBufferObject::generateRenderBufferObject(int newWidth, int newHeight) {
    setSize(newWidth, newHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBufferObjectMultiSample::generateRenderBufferObject(int newWidth, int newHeight) {
    setSize(newWidth, newHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
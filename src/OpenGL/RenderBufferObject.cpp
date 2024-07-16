#include "RenderBufferObject.h"

void RenderBufferObject::init() {
    glBindRenderbuffer(GL_RENDERBUFFER, rboID);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RenderBufferObjectMultiSample::init() {
    glBindRenderbuffer(GL_RENDERBUFFER, rboID);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, internalFormat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
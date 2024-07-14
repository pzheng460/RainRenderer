#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer() {
    framebuffer = 0;
}

FrameBuffer::FrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth) :
    numOfColorTextureAttachments(numOfColorTextureAttachments), numOfDepthTextureAttachments(numOfDepthTextureAttachments), numOfRenderBufferObjectDepth(numOfRenderBufferObjectDepth) {
    glGenFramebuffers(1, &framebuffer);
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        auto textureColorBufferPtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_COMMON_COLOR_ATTACHMENT);
        textureColorBuffers.push_back(std::shared_ptr<Texture>(textureColorBufferPtr));
    }
    if (numOfDepthTextureAttachments > 0) {
        for (int i = 0; i < numOfDepthTextureAttachments; i++) {
            auto textureDepthBufferPtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_COMMON_DEPTH_ATTACHMENT);
            textureDepthBuffer = std::shared_ptr<Texture>(textureDepthBufferPtr);
        }
    }
    if (numOfRenderBufferObjectDepth > 0) {
        auto renderBufferObject = RenderBufferObjectFactory::createRenderBufferObject(RenderBufferObjectFactoryType::RENDER_BUFFER_OBJECT_DEPTH);
        rboDepth = std::shared_ptr<RenderBufferObject>(renderBufferObject);
    }
}

void FrameBuffer::setSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;

    // create floating point color buffer 创建浮点颜色缓冲区
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        textureColorBuffers[i]->setSize(width, height);
    }
    // create depth texture 创建深度纹理
    if (numOfDepthTextureAttachments > 0) {
        textureDepthBuffer->setSize(width, height);
    }
    // create depth buffer (renderbuffer) 创建深度缓冲区（渲染缓冲区）
    if (numOfRenderBufferObjectDepth > 0) {
        rboDepth->setSize(width, height);
    }
}

void FrameBuffer::init() {
    bind();

    // create floating point color buffer 创建浮点颜色缓冲区
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        textureColorBuffers[i]->init(nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textureColorBuffers[i]->target, textureColorBuffers[i]->textureID, 0);
    }

    // create depth texture 创建深度纹理
    if (numOfDepthTextureAttachments > 0) {
        textureDepthBuffer->init(nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureDepthBuffer->target, textureDepthBuffer->textureID, 0);
    }

    // create depth buffer (renderbuffer) 创建深度缓冲区（渲染缓冲区）
    if (numOfRenderBufferObjectDepth > 0) {
        rboDepth->init();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth->rbo);
    }

    if (numOfColorTextureAttachments > 0) {
        // 动态设置绘制缓冲区
        std::vector<GLuint> attachments(numOfColorTextureAttachments);
        for (int i = 0; i < numOfColorTextureAttachments; ++i) {
            attachments[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glDrawBuffers(numOfColorTextureAttachments, attachments.data());
    }
    else
    {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    // check if frame buffer is complete 检查帧缓冲是否完整
    checkComplete();
    unbind();
}

void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::reset()
{
    glViewport(0, 0, width, height);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FrameBuffer::setColorTextureAttachment(Texture* texture, int i)
{
    textureColorBuffers[i] = std::shared_ptr<Texture>(texture);
}

void FrameBuffer::setDepthTextureAttachment(Texture* texture)
{
    textureDepthBuffer = std::shared_ptr<Texture>(texture);
}

void FrameBuffer::setRenderBufferDepthAttachment(RenderBufferObject* rbo)
{
    rboDepth = std::shared_ptr<RenderBufferObject>(rbo);
}

bool FrameBuffer::checkComplete()
{
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER::" << toString(frameBufferFactoryType) << " is not complete!" << std::endl;
        return false;
    }
    return true;
}

void FrameBuffer::transferFrameBuffer(FrameBuffer& dstFrameBuffer, GLenum target, int srcIndex, int dstIndex)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstFrameBuffer.framebuffer);

    if (target == GL_COLOR_BUFFER_BIT)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + srcIndex);
        glDrawBuffer(GL_COLOR_ATTACHMENT0 + dstIndex);
        glBlitFramebuffer(0, 0, width, height, 0, 0, dstFrameBuffer.width, dstFrameBuffer.height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }
    else if (target == GL_DEPTH_BUFFER_BIT)
    {
        glBlitFramebuffer(0, 0, width, height, 0, 0, dstFrameBuffer.width, dstFrameBuffer.height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }
    else
    {
        std::cerr << "FrameBuffer transferFrameBuffer target not found" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer* FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType frameBufferFactoryType)
{
    FrameBuffer* frameBuffer = nullptr;
    if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_DEFAULT)
    {
        frameBuffer = new FrameBuffer();
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_SHADOW_MAP)
    {
        frameBuffer = new FrameBuffer(0, 1, 0);
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_MSAA)
    {
        frameBuffer = new FrameBuffer(2, 0, 1);
        for (int i = 0; i < frameBuffer->numOfColorTextureAttachments; i++) {
            auto textureColorBufferPtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_MSAA_COLOR_ATTACHMENT);
            auto textureColorBuffer = std::shared_ptr<Texture>(textureColorBufferPtr);
            frameBuffer->textureColorBuffers[i] = std::move(textureColorBuffer);
        }
        auto rboDepth = RenderBufferObjectFactory::createRenderBufferObject(RenderBufferObjectFactoryType::RENDER_BUFFER_OBJECT_DEPTH_MULTI_SAMPLE);
        frameBuffer->rboDepth = std::shared_ptr<RenderBufferObject>(rboDepth);
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_INTERMEDIATE)
    {
        frameBuffer = new FrameBuffer(2, 0, 1);
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_PING_PONG)
    {
        frameBuffer = new FrameBuffer(1, 0, 0);
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_GEOMETRY)
    {
        frameBuffer = new FrameBuffer(3, 0, 1);
        auto texturePositionColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_GEOMETRY_POSITION_COLOR_ATTACHMENT);
        frameBuffer->textureColorBuffers[0] = std::shared_ptr<Texture>(texturePositionColorBuffer);
        auto textureNormalColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_GEOMETRY_NORMAL_COLOR_ATTACHMENT);
        frameBuffer->textureColorBuffers[1] = std::shared_ptr<Texture>(textureNormalColorBuffer);
        auto textureAlbedoSpecColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_GEOMETRY_ALBEDO_SPEC_COLOR_ATTACHMENT);
        frameBuffer->textureColorBuffers[2] = std::shared_ptr<Texture>(textureAlbedoSpecColorBuffer);
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_SSAO)
    {
        frameBuffer = new FrameBuffer(1, 0, 0);
        for (int i = 0; i < frameBuffer->numOfColorTextureAttachments; i++) {
            auto textureColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_SSAO_COLOR_ATTACHMENT);
            frameBuffer->textureColorBuffers[i] = std::shared_ptr<Texture>(textureColorBuffer);
        }
    }
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_SSAO_BLUR)
    {
        frameBuffer = new FrameBuffer(1, 0, 0);
        for (int i = 0; i < frameBuffer->numOfColorTextureAttachments; i++) {
            auto textureColorBuffer = TextureFactory::createTexture(TextureFactoryType::TEXTURE_SSAO_COLOR_ATTACHMENT);
            frameBuffer->textureColorBuffers[i] = std::shared_ptr<Texture>(textureColorBuffer);
        }
    }
    else
    {
        std::cerr << "FrameBufferFactoryType not found" << std::endl;
    }

    if (frameBuffer != nullptr) frameBuffer->frameBufferFactoryType = frameBufferFactoryType;
    return frameBuffer;
}
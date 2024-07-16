#include "FrameBuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer() {
    framebufferID = 0;
}

FrameBuffer::FrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth) :
    numOfColorTextureAttachments(numOfColorTextureAttachments), numOfDepthTextureAttachments(numOfDepthTextureAttachments), numOfRenderBufferObjectDepth(numOfRenderBufferObjectDepth) {
    glGenFramebuffers(1, &framebufferID);
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
}

void FrameBuffer::setAllSizes(int newWidth, int newHeight) {
    setSize(newWidth, newHeight);

    // create floating point color buffer 创建浮点颜色缓冲区
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        textureColorBuffers[i]->setSize(newWidth, newHeight);
    }
    // create depth texture 创建深度纹理
    if (numOfDepthTextureAttachments > 0) {
        textureDepthBuffer->setSize(newWidth, newHeight);
    }
    // create depth buffer (renderbuffer) 创建深度缓冲区（渲染缓冲区）
    if (numOfRenderBufferObjectDepth > 0) {
        rboDepth->setSize(newWidth, newHeight);
    }
}

void FrameBuffer::init() {
    // create floating point color buffer 创建浮点颜色缓冲区
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        if (textureColorBuffers[i]->textureType == TextureType::TEXTURE_2D)
        {
            dynamic_cast<Texture2D*>(textureColorBuffers[i].get())->init();
        }
        else if (textureColorBuffers[i]->textureType == TextureType::TEXTURE_2D_MULTISAMPLE)
        {
            dynamic_cast<Texture2DMultiSample*>(textureColorBuffers[i].get())->init();
        }
        else if (textureColorBuffers[i]->textureType == TextureType::TEXTURE_CUBE_MAP)
        {
            dynamic_cast<TextureCubeMap*>(textureColorBuffers[i].get())->init();
        }

        if (textureColorBuffers[i]->textureType != TextureType::TEXTURE_CUBE_MAP) {
            setColorTextureAttachment(i); // In default, we only set the 2d texture attachment
        }
    }

    // create depth texture 创建深度纹理
    if (numOfDepthTextureAttachments > 0) {
        dynamic_cast<Texture2D*>(textureDepthBuffer.get())->init();
        setDepthTextureAttachment();
    }

    // create depth buffer (renderbuffer) 创建深度缓冲区（渲染缓冲区）
    if (numOfRenderBufferObjectDepth > 0) {
        rboDepth->init();
        setRenderBufferObjectDepthAttachment();
    }

    bind();
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
    unbind();
}

void FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
}

void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::setViewPort() const {
    glViewport(0, 0, width, height);
}

void FrameBuffer::clear() const {
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

void FrameBuffer::reset() {
    setViewPort();
    clear();
}

void FrameBuffer::setColorTextureAttachment(int colorAttachmentIndex, int level) {
    bind();
    if (textureColorBuffers[colorAttachmentIndex]->textureType != TextureType::TEXTURE_CUBE_MAP) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, textureColorBuffers[colorAttachmentIndex]->target, textureColorBuffers[colorAttachmentIndex]->textureID, level);
    } else {
        std::cerr << "Can not set 2D texture with TEXTURE_CUBE_MAP" << std::endl;
    }
    checkComplete();
}

void FrameBuffer::setColorTextureAttachmentCubeFace(int faceIndex, int colorAttachmentIndex, int level)
{
    bind();
    if (textureColorBuffers[colorAttachmentIndex]->textureType == TextureType::TEXTURE_CUBE_MAP) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, textureColorBuffers[colorAttachmentIndex]->textureID, level);
    } else {
        std::cerr << "Can not set cube face without TEXTURE_CUBE_MAP" << std::endl;
    }
    checkComplete();
}

void FrameBuffer::setDepthTextureAttachment(int level) {
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureDepthBuffer->target, textureDepthBuffer->textureID, level);
    checkComplete();
}

void FrameBuffer::setRenderBufferObjectDepthAttachment() {
    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth->rboID);
    checkComplete();
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
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferID);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstFrameBuffer.framebufferID);

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
    else if (frameBufferFactoryType == FrameBufferFactoryType::FRAME_BUFFER_SKYBOX_CAPTURE)
    {
        frameBuffer = new FrameBuffer(1, 0, 1);
        auto rboDepthPtr = RenderBufferObjectFactory::createRenderBufferObject(RenderBufferObjectFactoryType::RENDER_BUFFER_OBJECT_DEPTH_SKYBOX);
        frameBuffer->rboDepth = std::shared_ptr<RenderBufferObject>(rboDepthPtr);
    }
    else
    {
        std::cerr << "FrameBufferFactoryType not found" << std::endl;
    }

    if (frameBuffer != nullptr) frameBuffer->frameBufferFactoryType = frameBufferFactoryType;
    return frameBuffer;
}
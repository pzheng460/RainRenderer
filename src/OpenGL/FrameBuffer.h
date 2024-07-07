#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Texture.h"
#include "RenderBufferObject.h"
#include "Common.h"

class FrameBuffer {
public:
    explicit FrameBuffer(int numOfColorTextureAttachments = 1, int numOfDepthTextureAttachments = 0, int numOfRenderBufferObjectDepth = 0);
    virtual ~FrameBuffer() {
        glDeleteFramebuffers(1, &framebuffer);
    }

    // 禁用拷贝构造函数和拷贝赋值运算符
    FrameBuffer(const FrameBuffer& other) = delete;
    FrameBuffer& operator=(const FrameBuffer& other) = delete;

    // 移动构造函数
    FrameBuffer(FrameBuffer&& other) noexcept
            : numOfColorTextureAttachments(other.numOfColorTextureAttachments),
              numOfDepthTextureAttachments(other.numOfDepthTextureAttachments),
              numOfRenderBufferObjectDepth(other.numOfRenderBufferObjectDepth),
              width(other.width),
              height(other.height),
              textureColorBuffers(std::move(other.textureColorBuffers)),
              textureDepthBuffer(std::move(other.textureDepthBuffer)),
              rboDepth(std::move(other.rboDepth)),
              frameBufferFactoryType(other.frameBufferFactoryType) {
        framebuffer = other.framebuffer;
        other.framebuffer = 0;
    }

    // 移动赋值运算符
    FrameBuffer& operator=(FrameBuffer&& other) noexcept {
        if (this != &other) {
            glDeleteFramebuffers(1, &framebuffer);
            numOfColorTextureAttachments = other.numOfColorTextureAttachments;
            numOfDepthTextureAttachments = other.numOfDepthTextureAttachments;
            numOfRenderBufferObjectDepth = other.numOfRenderBufferObjectDepth;
            width = other.width;
            height = other.height;
            textureColorBuffers = std::move(other.textureColorBuffers);
            textureDepthBuffer = std::move(other.textureDepthBuffer);
            rboDepth = std::move(other.rboDepth);
            frameBufferFactoryType = other.frameBufferFactoryType;
            framebuffer = other.framebuffer;
            other.framebuffer = 0;
        }
        return *this;
    }

    void setSize(int newWidth, int newHeight) {
        this->width = newWidth;
        this->height = newHeight;
    }

    void bind();
    void unbind();

    void bindColorTextureAttachment();
    void bindDepthTextureAttachment();
    void bindRenderBufferDepthAttachment();
    void generateFrameBuffer(int newWidth, int newHeight);

    bool checkComplete();

    void transferFrameBuffer(FrameBuffer& targetFrameBuffer);

    unsigned int framebuffer = 0;
    int numOfColorTextureAttachments;
    int numOfDepthTextureAttachments;
    int numOfRenderBufferObjectDepth;

    std::vector<std::shared_ptr<Texture>> textureColorBuffers;
    std::shared_ptr<Texture> textureDepthBuffer;
    std::shared_ptr<RenderBufferObject> rboDepth;

    FrameBufferFactoryType frameBufferFactoryType;

    int width, height;
};

class FrameBufferFactory {
public:
    static FrameBuffer createFrameBuffer(FrameBufferFactoryType frameBufferFactoryType);
};

#endif // FRAMEBUFFER_H

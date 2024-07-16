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
    explicit FrameBuffer();
    explicit FrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth);
    virtual ~FrameBuffer() {
        glDeleteFramebuffers(1, &framebufferID);
    }

    // delete copy constructor and assignment operator 禁用拷贝构造函数和拷贝赋值运算符
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
        framebufferID = other.framebufferID;
        other.framebufferID = 0;
    }

    // 移动赋值运算符
    FrameBuffer& operator=(FrameBuffer&& other) noexcept {
        if (this != &other) {
            glDeleteFramebuffers(1, &framebufferID);
            numOfColorTextureAttachments = other.numOfColorTextureAttachments;
            numOfDepthTextureAttachments = other.numOfDepthTextureAttachments;
            numOfRenderBufferObjectDepth = other.numOfRenderBufferObjectDepth;
            width = other.width;
            height = other.height;
            textureColorBuffers = std::move(other.textureColorBuffers);
            textureDepthBuffer = std::move(other.textureDepthBuffer);
            rboDepth = std::move(other.rboDepth);
            frameBufferFactoryType = other.frameBufferFactoryType;
            framebufferID = other.framebufferID;
            other.framebufferID = 0;
        }
        return *this;
    }

    void init();
    void setSize(int newWidth, int newHeight);
    void setAllSizes(int newWidth, int newHeight);

    void setColorTextureAttachment(int colorAttachmentIndex = 0, int level = 0);
    void setColorTextureAttachmentCubeFace(int faceIndex, int colorAttachmentIndex = 0, int level = 0);
    void setDepthTextureAttachment(int level = 0);
    void setRenderBufferObjectDepthAttachment();
    bool checkComplete();

    void bind();
    void unbind();

    void clear() const;
    void setViewPort() const;
    void reset();

    void transferFrameBuffer(FrameBuffer& dstFrameBuffer, GLenum target, int srcIndex = 0, int dstIndex = 0);

    unsigned int framebufferID = 0;
    int numOfColorTextureAttachments = 1;
    int numOfDepthTextureAttachments = 1;
    int numOfRenderBufferObjectDepth = 0;

    std::vector<std::shared_ptr<Texture>> textureColorBuffers;
    std::shared_ptr<Texture> textureDepthBuffer;
    std::shared_ptr<RenderBufferObject> rboDepth;

    FrameBufferFactoryType frameBufferFactoryType;

    int width, height;
};

class FrameBufferFactory {
public:
    static FrameBuffer* createFrameBuffer(FrameBufferFactoryType frameBufferFactoryType);
};

#endif // FRAMEBUFFER_H

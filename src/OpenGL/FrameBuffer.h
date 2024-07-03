#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Texture {
public:
    Texture() {
        glGenTextures(1, &texture);
    }
    virtual ~Texture() {
        glDeleteTextures(1, &texture);
    }
    virtual void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) = 0;
    unsigned int getTexture() const { return texture; }
protected:
    unsigned int texture;
};

class ColorTexture : public Texture {
public:
    virtual ~ColorTexture() = default;
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class DepthTexture : public Texture {
public:
    virtual ~DepthTexture() = default;
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class RenderBufferObject {
public:
    RenderBufferObject() {
        glGenRenderbuffers(1, &rbo);
    }
    virtual ~RenderBufferObject() {
        glDeleteRenderbuffers(1, &rbo);
    }
    virtual void generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT) = 0;
    unsigned int getRBO() const { return rbo; }
protected:
    unsigned int rbo;
};

class RenderBufferObjectDepth : public RenderBufferObject {
public:
    void generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class FrameBuffer {
public:
    FrameBuffer(int numOfColorTextureAttachments = 1, int numOfDepthTextureAttachments = 0, int numOfRenderBufferObjectDepth = 0);
    virtual ~FrameBuffer() {
        glDeleteFramebuffers(1, &framebuffer);
    }

    // 移动构造函数
    FrameBuffer(FrameBuffer&& other) noexcept
            : framebuffer(other.framebuffer),
              numOfColorTextureAttachments(other.numOfColorTextureAttachments),
              numOfDepthTextureAttachments(other.numOfDepthTextureAttachments),
              numOfRenderBufferObjectDepth(other.numOfRenderBufferObjectDepth),
              textureColorBuffers(std::move(other.textureColorBuffers)),
              textureDepthBuffer(std::move(other.textureDepthBuffer)),
              rboDepth(std::move(other.rboDepth)) {
        other.framebuffer = 0; // 重置源对象的帧缓冲ID
    }

    // 移动赋值运算符
    FrameBuffer& operator=(FrameBuffer&& other) noexcept {
        if (this != &other) {
            glDeleteFramebuffers(1, &framebuffer);

            framebuffer = other.framebuffer;
            numOfColorTextureAttachments = other.numOfColorTextureAttachments;
            numOfDepthTextureAttachments = other.numOfDepthTextureAttachments;
            numOfRenderBufferObjectDepth = other.numOfRenderBufferObjectDepth;
            textureColorBuffers = std::move(other.textureColorBuffers);
            textureDepthBuffer = std::move(other.textureDepthBuffer);
            rboDepth = std::move(other.rboDepth);

            other.framebuffer = 0; // 重置源对象的帧缓冲ID
        }
        return *this;
    }

    // 拷贝构造函数
    FrameBuffer(const FrameBuffer& other)
            : numOfColorTextureAttachments(other.numOfColorTextureAttachments),
              numOfDepthTextureAttachments(other.numOfDepthTextureAttachments),
              numOfRenderBufferObjectDepth(other.numOfRenderBufferObjectDepth) {
        glGenFramebuffers(1, &framebuffer);
        // 复制纹理和渲染缓冲对象
        for (const auto& tex : other.textureColorBuffers) {
            textureColorBuffers.push_back(std::make_unique<ColorTexture>(*tex));
        }
        if (other.textureDepthBuffer) {
            textureDepthBuffer = std::make_unique<DepthTexture>(*other.textureDepthBuffer);
        }
        if (other.rboDepth) {
            rboDepth = std::make_unique<RenderBufferObjectDepth>(*other.rboDepth);
        }
    }

    // 拷贝赋值运算符
    FrameBuffer& operator=(const FrameBuffer& other) {
        if (this != &other) {
            glDeleteFramebuffers(1, &framebuffer);
            glGenFramebuffers(1, &framebuffer);

            numOfColorTextureAttachments = other.numOfColorTextureAttachments;
            numOfDepthTextureAttachments = other.numOfDepthTextureAttachments;
            numOfRenderBufferObjectDepth = other.numOfRenderBufferObjectDepth;

            textureColorBuffers.clear();
            for (const auto& tex : other.textureColorBuffers) {
                textureColorBuffers.push_back(std::make_unique<ColorTexture>(*tex));
            }

            if (other.textureDepthBuffer) {
                textureDepthBuffer = std::make_unique<DepthTexture>(*other.textureDepthBuffer);
            } else {
                textureDepthBuffer.reset();
            }

            if (other.rboDepth) {
                rboDepth = std::make_unique<RenderBufferObjectDepth>(*other.rboDepth);
            } else {
                rboDepth.reset();
            }
        }
        return *this;
    }

    void init();
    void bind();
    void unbind();

    virtual void bindColorTextureAttachment();
    virtual void bindDepthTextureAttachment();
    virtual void bindRenderBufferDepthAttachment();
    virtual void generateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT);

    bool checkComplete();

    void transferFrameBuffer(FrameBuffer& targetFrameBuffer, int SCR_WIDTH, int SCR_HEIGHT);

    std::vector<std::unique_ptr<ColorTexture>>& getTextureColorBuffer() {
        return textureColorBuffers;
    }

    std::unique_ptr<DepthTexture>& getTextureDepthBuffer() {
        return textureDepthBuffer;
    }

    std::unique_ptr<RenderBufferObjectDepth>& getRboDepth() {
        return rboDepth;
    }

    int getNumOfColorTextureAttachments() {
        return numOfColorTextureAttachments;
    }

    int getNumOfDepthTextureAttachments() {
        return numOfDepthTextureAttachments;
    }

    unsigned int getFrameBuffer() {
        return framebuffer;
    }

protected:
    unsigned int framebuffer = 0;
    int numOfColorTextureAttachments;
    int numOfDepthTextureAttachments;
    int numOfRenderBufferObjectDepth;

    std::vector<std::unique_ptr<ColorTexture>> textureColorBuffers;
    std::unique_ptr<DepthTexture> textureDepthBuffer;
    std::unique_ptr<RenderBufferObjectDepth> rboDepth;
};

#endif // FRAMEBUFFER_H

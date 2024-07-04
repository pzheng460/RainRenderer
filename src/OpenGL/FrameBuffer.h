#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Texture.h"

class ColorTexture : public Texture {
public:
    ColorTexture() {
        internalFormat = GL_RGBA16F;
        format = GL_RGBA;
        type = GL_FLOAT;
    }
    virtual ~ColorTexture() = default;
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT, GLvoid* data) override;
};

class DepthTexture : public Texture {
public:
    virtual ~DepthTexture() = default;
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT, GLvoid* data) override;
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

    void init();
    void bind();
    void unbind();

    virtual void bindColorTextureAttachment();
    virtual void bindDepthTextureAttachment();
    virtual void bindRenderBufferDepthAttachment();
    virtual void generateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT);

    bool checkComplete();

    void transferFrameBuffer(FrameBuffer& targetFrameBuffer, int SCR_WIDTH, int SCR_HEIGHT);

    std::vector<std::shared_ptr<Texture>>& getTextureColorBuffer() {
        return textureColorBuffers;
    }

    std::shared_ptr<DepthTexture>& getTextureDepthBuffer() {
        return textureDepthBuffer;
    }

    std::shared_ptr<RenderBufferObject>& getRboDepth() {
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

    std::vector<std::shared_ptr<Texture>> textureColorBuffers;
    std::shared_ptr<DepthTexture> textureDepthBuffer;
    std::shared_ptr<RenderBufferObject> rboDepth;
};

#endif // FRAMEBUFFER_H

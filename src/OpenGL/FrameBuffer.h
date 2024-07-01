#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Textures {
public:
    Textures() = default;
    virtual void generateTexture(int SCR_WIDTH, int SCR_HEIGHT);
    unsigned int getTexture() {
        return texture;
    }
protected:
    unsigned int texture;
};

class RenderBufferObject {
public:
    RenderBufferObject() = default;
    virtual void generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT);
    unsigned int getRBO() { return rbo; }
protected:
    unsigned int rbo;
};

class RenderBufferObjectDepth : public RenderBufferObject {
public:
    RenderBufferObjectDepth() = default;
    void generateRenderBufferObject(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class FrameBuffer {
public:
    FrameBuffer(int numOfColorAttachments = 1, int numOfDepthAttachments = 0);
    void init();
    void bind();
    void unbind();
    virtual void bindColorTextureAttachment();
    virtual void bindRenderBufferDepthAttachment();

    bool checkComplete();

    void transferFrameBuffer(FrameBuffer& targetFrameBuffer, int SCR_WIDTH, int SCR_HEIGHT);

    virtual void generateFrameBuffer(int SCR_WIDTH, int SCR_HEIGHT);

    std::vector<std::unique_ptr<Textures>>& getTextureColorBuffer() {
        return textureColorBuffers;
    }

    int getNumOfColorAttachments() {
        return numOfColorAttachments;
    }

    int getNumOfDepthAttachments() {
        return numOfDepthAttachments;
    }

    unsigned int getFrameBuffer() {
        return framebuffer;
    }

protected:
    unsigned int framebuffer;
    int numOfColorAttachments;
    int numOfDepthAttachments;

    std::vector<std::unique_ptr<Textures>> textureColorBuffers;
    std::unique_ptr<RenderBufferObjectDepth> rboDepth;
};

#endif // FRAMEBUFFER_H

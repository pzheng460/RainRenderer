#ifndef RENDERBUFFEROBJECT_H
#define RENDERBUFFEROBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderBufferObject {
public:
    RenderBufferObject() {
        glGenRenderbuffers(1, &rbo);
    }
    virtual ~RenderBufferObject() {
        glDeleteRenderbuffers(1, &rbo);
    }
    virtual void generateRenderBufferObject(int newWidth, int newHeight) = 0;
    void setSize(int newWidth, int newHeight) {
        this->width = newWidth;
        this->height = newHeight;
    }
    unsigned int rbo;
    int width, height;
};

class RenderBufferObjectDepth : public RenderBufferObject {
public:
    void generateRenderBufferObject(int newWidth, int newHeight) override;
};

class RenderBufferObjectDepthMultiSample final : public RenderBufferObject {
public:
    void generateRenderBufferObject(int newWidth, int newHeight) override;
private:
    int samples = 4;
};

#endif // RENDERBUFFEROBJECT_H

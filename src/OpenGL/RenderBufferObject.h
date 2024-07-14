#ifndef RENDERBUFFEROBJECT_H
#define RENDERBUFFEROBJECT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Common.h"

class RenderBufferObject {
public:
    RenderBufferObject() {
        glGenRenderbuffers(1, &rbo);
    }
    virtual ~RenderBufferObject() {
        glDeleteRenderbuffers(1, &rbo);
    }
    virtual void init();
    void setSize(int newWidth, int newHeight) {
        this->width = newWidth;
        this->height = newHeight;
    }
    unsigned int rbo;
    int width, height;
    GLenum internalFormat;
    RenderBufferObjectType renderBufferObjectType = RenderBufferObjectType::RENDER_BUFFER_OBJECT;
    RenderBufferObjectFactoryType renderBufferObjectFactoryType;
};

class RenderBufferObjectMultiSample final : public RenderBufferObject {
public:
    RenderBufferObjectMultiSample() {
        renderBufferObjectType = RenderBufferObjectType::RENDER_BUFFER_OBJECT_MULTI_SAMPLE;
    }
    void init() override;
private:
    int samples = 4;
};

class RenderBufferObjectFactory {
public:
    static RenderBufferObject* createRenderBufferObject(RenderBufferObjectFactoryType renderBufferObjectFactoryType) {
        RenderBufferObject* renderBufferObject = nullptr;
        if (renderBufferObjectFactoryType == RenderBufferObjectFactoryType::RENDER_BUFFER_OBJECT_DEPTH)
        {
            renderBufferObject = new RenderBufferObject();
            renderBufferObject->internalFormat = GL_DEPTH_COMPONENT;
        }
        else if (renderBufferObjectFactoryType == RenderBufferObjectFactoryType::RENDER_BUFFER_OBJECT_DEPTH_MULTI_SAMPLE)
        {
            renderBufferObject = new RenderBufferObjectMultiSample();
            renderBufferObject->internalFormat = GL_DEPTH24_STENCIL8;
        }
        else
        {
            std::cerr << "ERROR::RENDER_BUFFER_OBJECT_FACTORY::createRenderBufferObject::Unknown RenderBufferObjectFactoryType" << std::endl;
        }
        if (renderBufferObject)
        {
            renderBufferObject->renderBufferObjectFactoryType = renderBufferObjectFactoryType;
        }
        return renderBufferObject;
    }
};

#endif // RENDERBUFFEROBJECT_H

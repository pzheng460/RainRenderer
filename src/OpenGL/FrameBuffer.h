#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class FrameBuffer {
public:
    FrameBuffer() = default;
    void init();
    void bind();
    void unbind();
    void createColorTextureAttachment(int SCR_WIDTH, int SCR_HEIGHT);
    void createRenderBufferAttachment(int SCR_WIDTH, int SCR_HEIGHT);
    void createMSAAColorTextureAttachment(int SCR_WIDTH, int SCR_HEIGHT);
    void createMSAARenderBufferAttachment(int SCR_WIDTH, int SCR_HEIGHT);
    bool checkComplete();

    void transferFrameBuffer(FrameBuffer& targetFrameBuffer, int SCR_WIDTH, int SCR_HEIGHT);

    unsigned int getTextureColorBuffer() const {
        return textureColorBuffer;
    }

protected:
    unsigned int framebuffer;
    unsigned int textureColorBuffer;
    unsigned int rbo;
};
#endif // FRAMEBUFFER_H

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class FrameBuffer {
public:
    FrameBuffer(int numOfColorAttachments = 1, int numOfDepthAttachments = 0);
    void init();
    void bind();
    void unbind();
    virtual void createColorTextureAttachment(int SCR_WIDTH, int SCR_HEIGHT);
    virtual void createRenderBufferAttachment(int SCR_WIDTH, int SCR_HEIGHT);

    bool checkComplete();

    void transferFrameBuffer(FrameBuffer& targetFrameBuffer, int SCR_WIDTH, int SCR_HEIGHT);

    std::vector<unsigned int>& getTextureColorBuffer() {
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
    std::vector<unsigned int> textureColorBuffers;
    unsigned int rbo;
    int numOfColorAttachments;
    int numOfDepthAttachments;
};
#endif // FRAMEBUFFER_H

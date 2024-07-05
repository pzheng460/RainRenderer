#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class Texture {
public:
    Texture() {
        glGenTextures(1, &textureID);
    }
    virtual ~Texture() {
        glDeleteTextures(1, &textureID);
    }

    virtual void specifyTexture(GLvoid* data) = 0;
    void generateTexture(int width, int height, GLvoid* data);
    void setSize(int width, int height) {
        this->width = width;
        this->height = height;
    }
    void setBorderColor(float r, float g, float b, float a) {
        borderColor[0] = r;
        borderColor[1] = g;
        borderColor[2] = b;
        borderColor[3] = a;
    }

    unsigned int textureID;
    GLint width, height;

    GLenum target; // GL_TEXTURE_2D, GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_CUBE_MAP
    GLint internalFormat; // GL_RGBA16F, GL_DEPTH_COMPONENT, GL_DEPTH24_STENCIL8
    GLenum format; // GL_RGBA, GL_DEPTH_COMPONENT, GL_DEPTH_STENCIL
    GLenum type; // GL_FLOAT, GL_UNSIGNED_BYTE

    GLenum minFilter = GL_NEAREST; // GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR
    GLenum magFilter = GL_NEAREST;
    GLenum wrapS = GL_REPEAT; // GL_REPEAT, GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT
    GLenum wrapT = GL_REPEAT;

    float borderColor[4] = { 0.0, 0.0, 0.0, 0.0 };

    std::string name;
};


#endif // TEXTURE_H

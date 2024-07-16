#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <utility>

#include "Common.h"

class Texture {
public:
    Texture() {
        glGenTextures(1, &textureID);
    }
    virtual ~Texture() {
        glDeleteTextures(1, &textureID);
    }

    void setSize(int newWidth, int newHeight);
    void setBorderColor(float r, float g, float b, float a) {
        borderColor[0] = r;
        borderColor[1] = g;
        borderColor[2] = b;
        borderColor[3] = a;
    }
    void setMipMap() const {
        glBindTexture(target, textureID);
        glGenerateMipmap(target);
        glBindTexture(target, 0);
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
    GLenum wrapR = GL_REPEAT;

    bool generateMipMap = false;

    float borderColor[4] = { 0.0, 0.0, 0.0, 0.0 };

    TextureType textureType;
    TextureFactoryType textureFactoryType;
    std::string variableName;
};

class Texture2D : public Texture {
public:
    Texture2D() {
        textureType = TextureType::TEXTURE_2D;
        target = GL_TEXTURE_2D;
    }
    void init(GLvoid* data = nullptr);
    std::string path;
};

class Texture2DMultiSample : public Texture {
public:
    Texture2DMultiSample() {
        textureType = TextureType::TEXTURE_2D_MULTISAMPLE;
        target = GL_TEXTURE_2D_MULTISAMPLE;
    }
    void init();
    int samples = 4;
};

class TextureCubeMap : public Texture {
public:
    TextureCubeMap() {
        textureType = TextureType::TEXTURE_CUBE_MAP;
        target = GL_TEXTURE_CUBE_MAP;
        paths.resize(6);
    }
    void init(const std::vector<GLvoid*>& dataSets = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr});
    std::vector<std::string> paths;
    std::vector<GLenum> subTargets = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };
};

class TextureFactory {
public:
    static Texture* createTexture(TextureFactoryType textureFactoryType);
};

#endif // TEXTURE_H

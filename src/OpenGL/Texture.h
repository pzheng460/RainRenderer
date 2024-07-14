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

    virtual void specifyTexture(GLvoid* data) = 0;
    void init(GLvoid* data);
    void setSize(int newWidth, int newHeight);
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
    GLenum wrapR = GL_REPEAT;

    bool generateMipMap = false;

    float borderColor[4] = { 0.0, 0.0, 0.0, 0.0 };

    std::string path;
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
    void specifyTexture(GLvoid* data) override;
};

class TextureMultiSample final : public Texture {
public:
    TextureMultiSample() {
        textureType = TextureType::TEXTURE_2D_MULTISAMPLE;
        target = GL_TEXTURE_2D_MULTISAMPLE;
    }
    void specifyTexture(GLvoid* data) override;
private:
    int samples = 4;
};

class TextureFactory {
public:
    static Texture* createTexture(TextureFactoryType textureFactoryType) {
        Texture* texture = nullptr;
        if (textureFactoryType == TextureFactoryType::TEXTURE_2D_LOADED) {
            texture = new Texture2D();

            texture->type = GL_UNSIGNED_BYTE;
            texture->minFilter = GL_REPEAT;
            texture->magFilter = GL_REPEAT;
            texture->wrapS = GL_LINEAR_MIPMAP_LINEAR;
            texture->wrapT = GL_LINEAR;
            texture->generateMipMap = true;
        } else if (textureFactoryType == TextureFactoryType::TEXTURE_MSAA_COLOR_ATTACHMENT) {
            texture = new TextureMultiSample();

            texture->internalFormat = GL_RGBA16F;
        } else if (textureFactoryType == TextureFactoryType::TEXTURE_COMMON_COLOR_ATTACHMENT) {
            texture = new Texture2D();

            texture->internalFormat = GL_RGBA16F;
            texture->format = GL_RGBA;
            texture->type = GL_FLOAT;

            texture->minFilter = GL_LINEAR;
            texture->magFilter = GL_LINEAR;
            texture->wrapS = GL_CLAMP_TO_EDGE;
            texture->wrapT = GL_CLAMP_TO_EDGE;
        } else if (textureFactoryType == TextureFactoryType::TEXTURE_COMMON_DEPTH_ATTACHMENT) {
            texture = new Texture2D();

            texture->internalFormat = GL_DEPTH_COMPONENT;
            texture->format = GL_DEPTH_COMPONENT;
            texture->type = GL_FLOAT;

            texture->minFilter = GL_NEAREST;
            texture->magFilter = GL_NEAREST;
            texture->wrapS = GL_CLAMP_TO_BORDER;
            texture->wrapT = GL_CLAMP_TO_BORDER;
            texture->setBorderColor(1.0f, 1.0f, 1.0f, 1.0f);
        } else if (textureFactoryType == TextureFactoryType::TEXTURE_GEOMETRY_POSITION_COLOR_ATTACHMENT) {
            texture = new Texture2D();

            texture->internalFormat = GL_RGBA16F;
            texture->format = GL_RGBA;
            texture->type = GL_FLOAT;

            texture->minFilter = GL_NEAREST;
            texture->magFilter = GL_NEAREST;
            texture->wrapS = GL_CLAMP_TO_EDGE;
            texture->wrapT = GL_CLAMP_TO_EDGE;
        } else if (textureFactoryType == TextureFactoryType::TEXTURE_GEOMETRY_NORMAL_COLOR_ATTACHMENT) {
            texture = new Texture2D();

            texture->internalFormat = GL_RGBA16F;
            texture->format = GL_RGBA;
            texture->type = GL_FLOAT;
        } else if (textureFactoryType == TextureFactoryType::TEXTURE_GEOMETRY_ALBEDO_SPEC_COLOR_ATTACHMENT) {
            texture = new Texture2D();

            texture->internalFormat = GL_RGBA;
            texture->format = GL_RGBA;
            texture->type = GL_UNSIGNED_BYTE;
        } else if (textureFactoryType == TextureFactoryType::TEXTURE_SSAO_NOISE) {
            texture = new Texture2D();

            texture->internalFormat = GL_RGB16F;
            texture->format = GL_RGB;
            texture->type = GL_FLOAT;
        } else if (textureFactoryType == TextureFactoryType::TEXTURE_SSAO_COLOR_ATTACHMENT) {
            texture = new Texture2D();

            texture->internalFormat = GL_RED;
            texture->format = GL_RED;
            texture->type = GL_FLOAT;

            texture->minFilter = GL_NEAREST;
            texture->magFilter = GL_NEAREST;
            texture->wrapS = GL_CLAMP_TO_EDGE;
            texture->wrapT = GL_CLAMP_TO_EDGE;
        } else {
            std::cerr << "TextureFactoryType not found!" << std::endl;
        }
        if (texture != nullptr) texture->textureFactoryType = textureFactoryType;
        return texture;
    }
};

#endif // TEXTURE_H

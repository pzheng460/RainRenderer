#include "Texture.h"

void Texture::setSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
}

void Texture2D::init(GLvoid *data) {
    glBindTexture(target, textureID);

    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);

    glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);

    if (generateMipMap) {
        setMipMap();
    }

    glBindTexture(target, 0);
}

void Texture2DMultiSample::init() {
    glBindTexture(target, textureID);

    glTexImage2DMultisample(target, samples, internalFormat, width, height, GL_TRUE);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);

    glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);

    if (generateMipMap) {
        setMipMap();
    }

    glBindTexture(target, 0);
}

void TextureCubeMap::init(const std::vector<GLvoid*>& dataSets) {
    assert(dataSets.size() == 6 && "dataSet's size must be 6");
    glBindTexture(target, textureID);

    for (unsigned int i = 0; i < dataSets.size(); i++) {
        glTexImage2D(subTargets[i], 0, internalFormat, width, height, 0, format, type, dataSets[i]);
    }

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapR);

    glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);

    if (generateMipMap) {
        setMipMap();
    }

    glBindTexture(target, 0);
}

Texture *TextureFactory::createTexture(TextureFactoryType textureFactoryType) {
    Texture* texture = nullptr;
    if (textureFactoryType == TextureFactoryType::TEXTURE_2D_LOADED)
    {
        texture = new Texture2D();

        texture->type = GL_UNSIGNED_BYTE;
        texture->minFilter = GL_REPEAT;
        texture->magFilter = GL_REPEAT;
        texture->wrapS = GL_LINEAR_MIPMAP_LINEAR;
        texture->wrapT = GL_LINEAR;
        texture->generateMipMap = true;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_2D_HDR_LOADED)
    {
        texture = new Texture2D();

        texture->internalFormat = GL_RGB16F;
        texture->format = GL_RGB;
        texture->type = GL_FLOAT;
        texture->minFilter = GL_LINEAR;
        texture->magFilter = GL_LINEAR;
        texture->wrapS = GL_CLAMP_TO_EDGE;
        texture->wrapT = GL_CLAMP_TO_EDGE;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_CUBE_MAP_SKYBOX)
    {
        texture = new TextureCubeMap();

        texture->internalFormat = GL_RGB16F;
        texture->format = GL_RGB;
        texture->type = GL_FLOAT;

        texture->minFilter = GL_LINEAR_MIPMAP_LINEAR;
        texture->magFilter = GL_LINEAR;
        texture->wrapS = GL_CLAMP_TO_EDGE;
        texture->wrapT = GL_CLAMP_TO_EDGE;
        texture->wrapR = GL_CLAMP_TO_EDGE;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_CUBE_LOADED)
    {
        texture = new TextureCubeMap();

        texture->internalFormat = GL_RGB;
        texture->format = GL_RGB;
        texture->type = GL_UNSIGNED_BYTE;

        texture->minFilter = GL_LINEAR;
        texture->magFilter = GL_LINEAR;
        texture->wrapS = GL_CLAMP_TO_EDGE;
        texture->wrapT = GL_CLAMP_TO_EDGE;
        texture->wrapR = GL_CLAMP_TO_EDGE;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_MSAA_COLOR_ATTACHMENT)
    {
        texture = new Texture2DMultiSample();

        texture->internalFormat = GL_RGBA16F;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_COMMON_COLOR_ATTACHMENT)
    {
        texture = new Texture2D();

        texture->internalFormat = GL_RGBA16F;
        texture->format = GL_RGBA;
        texture->type = GL_FLOAT;

        texture->minFilter = GL_LINEAR;
        texture->magFilter = GL_LINEAR;
        texture->wrapS = GL_CLAMP_TO_EDGE;
        texture->wrapT = GL_CLAMP_TO_EDGE;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_COMMON_DEPTH_ATTACHMENT)
    {
        texture = new Texture2D();

        texture->internalFormat = GL_DEPTH_COMPONENT;
        texture->format = GL_DEPTH_COMPONENT;
        texture->type = GL_FLOAT;

        texture->minFilter = GL_NEAREST;
        texture->magFilter = GL_NEAREST;
        texture->wrapS = GL_CLAMP_TO_BORDER;
        texture->wrapT = GL_CLAMP_TO_BORDER;
        texture->setBorderColor(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_GEOMETRY_POSITION_COLOR_ATTACHMENT)
    {
        texture = new Texture2D();

        texture->internalFormat = GL_RGBA16F;
        texture->format = GL_RGBA;
        texture->type = GL_FLOAT;

        texture->minFilter = GL_NEAREST;
        texture->magFilter = GL_NEAREST;
        texture->wrapS = GL_CLAMP_TO_EDGE;
        texture->wrapT = GL_CLAMP_TO_EDGE;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_GEOMETRY_NORMAL_COLOR_ATTACHMENT)
    {
        texture = new Texture2D();

        texture->internalFormat = GL_RGBA16F;
        texture->format = GL_RGBA;
        texture->type = GL_FLOAT;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_GEOMETRY_ALBEDO_SPEC_COLOR_ATTACHMENT)
    {
        texture = new Texture2D();

        texture->internalFormat = GL_RGBA;
        texture->format = GL_RGBA;
        texture->type = GL_UNSIGNED_BYTE;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_SSAO_NOISE)
    {
        texture = new Texture2D();

        texture->internalFormat = GL_RGB16F;
        texture->format = GL_RGB;
        texture->type = GL_FLOAT;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_SSAO_COLOR_ATTACHMENT)
    {
        texture = new Texture2D();

        texture->internalFormat = GL_RED;
        texture->format = GL_RED;
        texture->type = GL_FLOAT;

        texture->minFilter = GL_NEAREST;
        texture->magFilter = GL_NEAREST;
        texture->wrapS = GL_CLAMP_TO_EDGE;
        texture->wrapT = GL_CLAMP_TO_EDGE;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_IRRADIANCE)
    {
        texture = new TextureCubeMap();

        texture->internalFormat = GL_RGB16F;
        texture->format = GL_RGB;
        texture->type = GL_FLOAT;

        texture->minFilter = GL_LINEAR;
        texture->magFilter = GL_LINEAR;
        texture->wrapS = GL_CLAMP_TO_EDGE;
        texture->wrapT = GL_CLAMP_TO_EDGE;
        texture->wrapR = GL_CLAMP_TO_EDGE;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_PREFILTER)
    {
        texture = new TextureCubeMap();

        texture->internalFormat = GL_RGB16F;
        texture->format = GL_RGB;
        texture->type = GL_FLOAT;

        texture->minFilter = GL_LINEAR_MIPMAP_LINEAR;
        texture->magFilter = GL_LINEAR;
        texture->wrapS = GL_CLAMP_TO_EDGE;
        texture->wrapT = GL_CLAMP_TO_EDGE;
        texture->wrapR = GL_CLAMP_TO_EDGE;

        texture->generateMipMap = true;
    }
    else if (textureFactoryType == TextureFactoryType::TEXTURE_BRDF_LUT)
    {
        texture = new Texture2D();

        texture->internalFormat = GL_RG16F;
        texture->format = GL_RG;
        texture->type = GL_FLOAT;

        texture->minFilter = GL_LINEAR;
        texture->magFilter = GL_LINEAR;
        texture->wrapS = GL_CLAMP_TO_EDGE;
        texture->wrapT = GL_CLAMP_TO_EDGE;
    }
    else
    {
        std::cerr << toString(textureFactoryType) << " not found!" << std::endl;
    }
    if (texture != nullptr) texture->textureFactoryType = textureFactoryType;
    return texture;
}


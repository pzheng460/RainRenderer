#include "Utils.h"
#include <stb_image.h>

Texture* loadTexture2D(const std::string& path, bool flip) {
    stbi_set_flip_vertically_on_load(flip);
    // generate texture 创建纹理对象
    auto textureLoaded = TextureFactory::createTexture(TextureFactoryType::TEXTURE_2D_LOADED);
    dynamic_cast<Texture2D*>(textureLoaded)->path = path;

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        if (nrComponents == 1) {
            textureLoaded->format = textureLoaded->internalFormat = GL_RED; // 灰度图
        } else if (nrComponents == 3) {
            textureLoaded->format = textureLoaded->internalFormat = GL_RGB; // RGB图
        } else if (nrComponents == 4) {
            textureLoaded->format = textureLoaded->internalFormat = GL_RGBA; // RGBA图（带alpha值的透明通道）
        }
        textureLoaded->setSize(width, height);
        dynamic_cast<Texture2D*>(textureLoaded)->init(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }
    // free memory 释放图像数据
    stbi_image_free(data);

    return textureLoaded;
}

Texture* loadTexture2DHDR(const std::string& path, bool flip) {
    stbi_set_flip_vertically_on_load(flip);
    // generate texture 创建纹理对象
    auto textureLoaded = TextureFactory::createTexture(TextureFactoryType::TEXTURE_2D_HDR_LOADED);
    dynamic_cast<Texture2D*>(textureLoaded)->path = path;

    int width, height, nrComponents;
    float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        textureLoaded->setSize(width, height);
        dynamic_cast<Texture2D*>(textureLoaded)->init(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
    }
    // free memory 释放图像数据
    stbi_image_free(data);

    return textureLoaded;
}

Texture* loadTextureCube(const std::vector<std::string>& paths, bool flip) {
    stbi_set_flip_vertically_on_load(flip);
    auto textureLoaded = TextureFactory::createTexture(TextureFactoryType::TEXTURE_CUBE_LOADED);
    dynamic_cast<TextureCubeMap*>(textureLoaded)->paths = paths;

    std::vector<GLvoid*> dataSets;
    int width, height, nrChannels;
    for (unsigned int i = 0; i < paths.size(); i++)
    {
        unsigned char *data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            dataSets.push_back(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << paths[i] << std::endl;
        }
    }

    textureLoaded->setSize(width, height);
    dynamic_cast<TextureCubeMap*>(textureLoaded)->init(dataSets);

    for (unsigned int i = 0; i < dataSets.size(); i++)
    {
        stbi_image_free(dataSets[i]);
    }

    return textureLoaded;
}

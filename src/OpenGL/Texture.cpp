#include "Texture.h"

void Texture::generateTexture(int newWidth, int newHeight, GLvoid *data) {
    setSize(newWidth, newHeight);

    glBindTexture(target, textureID);

    specifyTexture(data);
    if (generateMipMap) {
        glGenerateMipmap(target);
    }
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);
    if (target == GL_TEXTURE_CUBE_MAP) {
        glTexParameteri(target, GL_TEXTURE_WRAP_R, wrapR);
    }

    glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindTexture(target, 0);
}

void Texture2D::specifyTexture(GLvoid *data) {
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
}

void TextureMultiSample::specifyTexture(GLvoid *data) {
    glTexImage2DMultisample(target, samples, internalFormat, width, height, GL_TRUE);
}


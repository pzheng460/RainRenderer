#include "Texture.h"

void Texture::generateTexture(int width, int height, GLvoid *data) {
    setSize(width, height);

    glBindTexture(target, textureID);

    specifyTexture(data);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, wrapT);

    glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindTexture(target, 0);
}
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture {
public:
    Texture() {
        glGenTextures(1, &texture);
    }
    virtual ~Texture() {
        glDeleteTextures(1, &texture);
    }
    virtual void generateTexture(int SCR_WIDTH, int SCR_HEIGHT, GLvoid* data) = 0;
    unsigned int getTexture() const { return texture; }

    unsigned int texture;
    GLint width, height;

    GLint internalFormat;
    GLenum format;
    GLenum type;
};


#endif // TEXTURE_H

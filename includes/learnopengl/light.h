#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <learnopengl/filesystem.h>
#include <learnopengl/camera.h>

class Light {
public:
    Light(const glm::vec3& position, const glm::vec3& color)
            : position(position), color(color),
              lightShader(FileSystem::getPath("src/OpenGL/shaders/light_cube.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/light_cube.fs").c_str()) {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
                -0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f,  0.5f, -0.5f,
                0.5f,  0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f, -0.5f,  0.5f,
                0.5f, -0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,

                -0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,

                0.5f,  0.5f,  0.5f,
                0.5f,  0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, -0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,

                -0.5f, -0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                0.5f, -0.5f,  0.5f,
                0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f,  0.5f, -0.5f,
                0.5f,  0.5f, -0.5f,
                0.5f,  0.5f,  0.5f,
                0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f,
        };
        // configure the cube's VAO (and VBO)
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);

        glGenBuffers(1, &lightVBO);
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void draw() {
        lightShader.use();
        lightShader.setVec3("lightColor", color);

        // MVP matrices
        lightShader.setMat4("model", modelMatrix);
        lightShader.setMat4("view", viewMatrix);
        lightShader.setMat4("projection", projectionMatrix);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT) {
        modelMatrix = glm::translate(glm::mat4(1.0f), position);
        viewMatrix = camera.GetViewMatrix();
        projectionMatrix = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    }

    void setPosition(const glm::vec3& position) { this->position = position; }
    void setColor(const glm::vec3& color) { this->color = color; }
    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getColor() const { return color; }
    friend class GUI;
private:
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix {};
    glm::mat4 projectionMatrix {};

    Shader lightShader;
    unsigned int lightVAO, lightVBO;
    glm::vec3 position;
    glm::vec3 color;
};

#endif // LIGHT_H

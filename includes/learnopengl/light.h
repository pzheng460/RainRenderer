#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <learnopengl/filesystem.h>
#include <learnopengl/camera.h>

const glm::vec3 AMBIENT = glm::vec3(0.5);
const glm::vec3 DIFFUSE = glm::vec3(0.5);
const glm::vec3 SPECULAR = glm::vec3(1.0);

class Light {
public:
    Light(const glm::vec3& position, const glm::vec3& color)
            : position(position), color(color), ambient(AMBIENT), diffuse(DIFFUSE), specular(SPECULAR),
              lightShader(FileSystem::getPath("src/OpenGL/shaders/light_cube.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/light_cube.fs").c_str()) {
        updateLightColor();
        updateLightSpaceMatrix();
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

    void updateLightColor() {
        ambientColor = color * ambient;
        diffuseColor = color * diffuse;
        specularColor = color * specular;
    }

    void updateLightSpaceMatrix() {
        lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);
        lightView = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
    }

    void setPosition(const glm::vec3& position) { this->position = position; }
    void setColor(const glm::vec3& color) {
        this->color = color;
        updateLightColor();
    }
    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getColor() const { return color; }
    const glm::vec3& getAmbientColor() const { return ambientColor; }
    const glm::vec3& getDiffuseColor() const { return diffuseColor; }
    const glm::vec3& getSpecularColor() const { return specularColor; }
    const glm::mat4& getLightSpaceMatrix() const { return lightSpaceMatrix; }
    friend class GUI;
private:
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix {};
    glm::mat4 projectionMatrix {};

    Shader lightShader;
    unsigned int lightVAO, lightVBO;
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 ambientColor;
    glm::vec3 diffuse;
    glm::vec3 diffuseColor;
    glm::vec3 specular;
    glm::vec3 specularColor;

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 100.0f;
};

#endif // LIGHT_H

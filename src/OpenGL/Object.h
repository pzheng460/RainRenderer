#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <learnopengl/light.h>
#include <stb_image.h>
#include <string>

enum ImplicitGeometryType {
    MODEL,
    SPHERE,
    CUBE,
    QUAD,
    PLANE
};

class Object {
public:
    Object() = default;
    Object(const Model& model, const Shader& shader);
    Object(ImplicitGeometryType geometryType, const Shader& shader, bool initializeTextures = true, std::string texturePath = "");
    virtual ~Object() = default;

    void setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);
    void setShader(const Shader& shader) { this->shader = shader; }  // 设置着色器（Shader）
    void setScale(float scale) { this->scale = scale; }  // 设置缩放

    void basicShaderSetting();
    void phongShaderSetting(Camera& camera, std::vector<Light>& light);
    virtual void draw();

    const glm::mat4 getModelMatrix() const {
        return modelMatrix;
    }

    const glm::mat4 getViewMatrix() const {
        return viewMatrix;
    }

    const glm::mat4 getProjectionMatrix() const {
        return projectionMatrix;
    }

protected:
    void generateModel(ImplicitGeometryType geometryType);
    void loadTextures(std::vector<Texture>& textures,
                      const std::string& diffusePath);
    Texture loadTexture(const std::string& path, const std::string& typeName);
    Shader shader;
    Model model;
    ImplicitGeometryType geometryType;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix {};
    glm::mat4 projectionMatrix {};

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float scale = 1.0f;
};

#endif // OBJECT_H
#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utils/shader.h>
#include <utils/camera.h>
#include <utils/model.h>
#include <utils/light.h>
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
    Object(const AssimpModel::Model& model);
    Object(ImplicitGeometryType geometryType, bool initializeTextures = true, std::string texturePath = "");
    virtual ~Object() = default;

    void setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);
    virtual void draw(Shader& shader);

    const glm::mat4 getModelMatrix() const {
        return modelMatrix;
    }

    const glm::mat4 getViewMatrix() const {
        return viewMatrix;
    }

    const glm::mat4 getProjectionMatrix() const {
        return projectionMatrix;
    }

    friend class GUI;

protected:
    void generateModel(ImplicitGeometryType geometryType);
    void loadTextures(std::vector<AssimpModel::Texture>& textures,
                      const std::string& diffusePath);
    AssimpModel::Texture loadTexture(const std::string& path, const std::string& typeName);
    AssimpModel::Model model;
    ImplicitGeometryType geometryType;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix {};
    glm::mat4 projectionMatrix {};

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float scale = 1.0f;
};

#endif // OBJECT_H
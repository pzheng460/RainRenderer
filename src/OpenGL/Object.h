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
#include "Geometry.h"
#include "FrameBuffer.h"

struct GeometryModel {
    std::shared_ptr<Geometry> geometry;
    std::vector<AssimpModel::Texture> textures;
};

class Object {
public:
    Object() = default;
    Object(const AssimpModel::Model& model);
    Object(GeometryType geometryType, bool initializeTextures = false, std::string texturePath = "", std::string name = "material.texture_diffuse1");
    virtual ~Object() = default;

    void setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);
    void addTexture(std::string name, const char* path);
    void addTexture(std::string name, unsigned int textureID);
    void draw(Shader& shader, bool setTexture = false);

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
    unsigned int loadTexture(const char *path);
    std::variant<AssimpModel::Model, GeometryModel> modelVariant;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix {};
    glm::mat4 projectionMatrix {};

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float scale = 1.0f;
};

#endif // OBJECT_H
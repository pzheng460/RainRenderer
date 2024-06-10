#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <stb_image.h>
#include <string>

enum ImplicitGeometryType {
    SPHERE,
    CUBE,
    QUAD
};

enum RenderMode {
    PBR,
    NORMAL
};

class Object {
public:
    Object(const Model& model, RenderMode renderMode);
    Object(ImplicitGeometryType geometryType, RenderMode renderMode);
    void setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);

    // 使用 std::variant 来存储 Model 或 ImplicitGeometryType
    std::variant<Model, ImplicitGeometryType> data;

    virtual unsigned int getAlbedoMap() const { return 0; }  // 虚函数
    virtual unsigned int getNormalMap() const { return 0; }
    virtual unsigned int getMetallicMap() const { return 0; }
    virtual unsigned int getRoughnessMap() const { return 0; }
    virtual unsigned int getAOMap() const { return 0; }

    const RenderMode getRenderMode() const {
        return renderMode;
    }

    const glm::mat4 getModelMatrix() const {
        return modelMatrix;
    }

    const glm::mat4 getViewMatrix() const {
        return viewMatrix;
    }

    const glm::mat4 getProjectionMatrix() const {
        return projectionMatrix;
    }

private:
    RenderMode renderMode;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix {};
    glm::mat4 projectionMatrix {};
};

#endif // OBJECT_H
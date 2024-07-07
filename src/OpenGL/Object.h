#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <string>
#include "Geometry.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"

class Object {
public:
    Object() = default;
    explicit Object(Model& model) : model(model) {}
    virtual ~Object() = default;

    void setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);
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
    Model model;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix {};
    glm::mat4 projectionMatrix {};

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float scale = 1.0f;
};

#endif // OBJECT_H
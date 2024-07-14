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

class Object {
public:
    Object() = default;
    explicit Object(Model& model) : model(model) {}
    virtual ~Object() = default;

    void draw(Shader* shader = nullptr);

    const glm::mat4 getModelMatrix() const;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float scale = 1.0f;

    friend class GUI;

protected:
    Model model;
};

#endif // OBJECT_H
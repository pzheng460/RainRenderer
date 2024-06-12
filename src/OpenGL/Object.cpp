#include "Object.h"
#include "render_implicit_geometry.h"

Object::Object(const Model& model, const Shader& shader)
        : data(model), shader(shader) {
}

Object::Object(ImplicitGeometryType geometryType, const Shader& shader)
        : data(geometryType), shader(shader) {
}

void Object::setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT) {
    // MVP matrices
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    viewMatrix = camera.GetViewMatrix();
    // 参数：fov视野，aspect宽高比，zNear近平面，zFar远平面
    projectionMatrix = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
}

void Object::draw() {
    shader.use();

    // MVP matrices
    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);

    // check types 进行类型检查
    if (std::holds_alternative<Model>(data)) {
        std::get<Model>(data).Draw(shader);
    } else {
        ImplicitGeometryType geometryType = std::get<ImplicitGeometryType>(data);
        renderGeometry(geometryType);
    }
}

void Object::renderGeometry(ImplicitGeometryType geometryType) {
    switch (geometryType) {
        case SPHERE:
            renderSphere();
            break;
        case CUBE:
            renderCube();
            break;
        case QUAD:
            renderQuad();
            break;
        default:
            std::cerr << "Unknown geometry type" << std::endl;
            break;
    }
}

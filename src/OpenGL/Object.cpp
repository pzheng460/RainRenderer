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
    // check types 进行类型检查
    if (std::holds_alternative<Model>(data)) {
        std::get<Model>(data).Draw(shader);
    } else {
        ImplicitGeometryType geometryType = std::get<ImplicitGeometryType>(data);
        renderGeometry(geometryType);
    }
}

void Object::basicShaderSetting() {
    shader.use();
    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);
}

void Object::phongShaderSetting(Camera& camera, std::vector<Light>& lights) {
    shader.use();

//    // directional light
//    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
//    shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
//    shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
//    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
//
//    // spotLight
//    shader.setVec3("spotLight.position", camera.Position);
//    shader.setVec3("spotLight.direction", camera.Front);
//    shader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
//    shader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
//    shader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
//    shader.setFloat("spotLight.constant", 1.0f);
//    shader.setFloat("spotLight.linear", 0.09f);
//    shader.setFloat("spotLight.quadratic", 0.032f);
//    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
//    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        shader.setVec3("pointLights[" + std::to_string(i) + "].position", lights[i].getPosition());
        shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", lights[i].getAmbientColor());
        shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", lights[i].getDiffuseColor());
        shader.setVec3("pointLights[" + std::to_string(i) + "].specular", lights[i].getSpecularColor());

        shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
        shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
        shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
    }

    shader.setVec3("viewPos", camera.Position);
    shader.setFloat("material.shininess", 32.0f);

    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);
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

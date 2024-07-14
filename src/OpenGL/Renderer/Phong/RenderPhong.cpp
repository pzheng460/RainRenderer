#include "../../Renderer.h"

void Renderer::renderPhong() {
    frameBufferMSAA->bind();
    frameBufferMSAA->reset();
        shaderPhong->use();

        for (unsigned int i = 0; i < scene.lights.size(); ++i)
        {
            shaderPhong->setVec3("pointLights[" + std::to_string(i) + "].position", scene.lights[i]->position);
            shaderPhong->setVec3("pointLights[" + std::to_string(i) + "].ambient", scene.lights[i]->getAmbientColor());
            shaderPhong->setVec3("pointLights[" + std::to_string(i) + "].diffuse", scene.lights[i]->getDiffuseColor());
            shaderPhong->setVec3("pointLights[" + std::to_string(i) + "].specular", scene.lights[i]->getSpecularColor());

            shaderPhong->setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            shaderPhong->setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
            shaderPhong->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
        }

        shaderPhong->setVec3("viewPos", scene.camera->Position);
        shaderPhong->setFloat("material.shininess", 64.0f);
        shaderPhong->setInt("size", static_cast<int>(scene.lights.size()));

        shaderPhong->setMat4("view", scene.camera->getViewMatrix());
        shaderPhong->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        if (gui.floorActive) {
            shaderPhong->setMat4("model", scene.floor->getModelMatrix());

            scene.floor->draw(shaderPhong.get());
        }

        for (auto& object : scene.objects) {
            shaderPhong->setMat4("model", object->getModelMatrix());

            object->draw(shaderPhong.get());
        }
    frameBufferMSAA->unbind();
}

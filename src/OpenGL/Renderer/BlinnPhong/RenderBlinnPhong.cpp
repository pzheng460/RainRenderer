#include "../../Renderer.h"

void Renderer::renderBlinnPhong(FrameBuffer* frameBuffer) {
    frameBufferMSAA->bind();
    frameBufferMSAA->reset();
        shaderBlinnPhong->use();

        if (gui.shadowActive) {
            shaderBlinnPhong->setBool("shadowActive", gui.shadowActive);
        }

        for (unsigned int i = 0; i < scene.lights.size(); ++i)
        {
            if (gui.shadowActive) {
                shaderBlinnPhong->setTexture("shadowMaps[" + std::to_string(i) + "]", frameBufferShadowMaps[i]->textureDepthBuffer.get());
                shaderBlinnPhong->setMat4("lightSpaceMatrix[" + std::to_string(i) + "]", scene.lights[i]->getLightSpaceMatrix());
            }
            shaderBlinnPhong->setVec3("pointLights[" + std::to_string(i) + "].position", scene.lights[i]->position);
            shaderBlinnPhong->setVec3("pointLights[" + std::to_string(i) + "].ambient", scene.lights[i]->getAmbientColor());
            shaderBlinnPhong->setVec3("pointLights[" + std::to_string(i) + "].diffuse", scene.lights[i]->getDiffuseColor());
            shaderBlinnPhong->setVec3("pointLights[" + std::to_string(i) + "].specular", scene.lights[i]->getSpecularColor());

            shaderBlinnPhong->setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
            shaderBlinnPhong->setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
            shaderBlinnPhong->setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
        }

        shaderBlinnPhong->setVec3("viewPos", scene.camera->Position);
        shaderBlinnPhong->setFloat("material.shininess", 64.0f);
        shaderBlinnPhong->setInt("size", static_cast<int>(scene.lights.size()));

        shaderBlinnPhong->setMat4("view", scene.camera->getViewMatrix());
        shaderBlinnPhong->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        if (gui.floorActive) {
            shaderBlinnPhong->setMat4("model", scene.floor->getModelMatrix());
            scene.floor->draw(shaderBlinnPhong.get());
        }

        for (auto& object : scene.objects) {
            shaderBlinnPhong->setMat4("model", object->getModelMatrix());
            object->draw(shaderBlinnPhong.get());
        }
    frameBuffer->unbind();
}

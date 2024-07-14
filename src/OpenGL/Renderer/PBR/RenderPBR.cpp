#include "../../Renderer.h"

void Renderer::renderPBR() {
    frameBufferMSAA->bind();
    frameBufferMSAA->reset();
        shaderPBR->use();

//        shaderPBR->setTexture("irradianceMap", irradianceMap);
//        shaderPBR->setTexture("prefilterMap", prefilterMap);
//        shaderPBR->setTexture("brdfLUT", brdfLUTTexture);
        for (int i = 0; i < scene.lights.size(); ++i)
        {
            shaderPBR->setVec3("lightPositions[" + std::to_string(i) + "]", scene.lights[i]->position);
            shaderPBR->setVec3("lightColors[" + std::to_string(i) + "]", scene.lights[i]->getColor());
        }

        shaderPBR->setMat4("view", scene.camera->getViewMatrix());
        shaderPBR->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        for (auto& object : scene.objects) {
            shaderPBR->setMat4("model", object->getModelMatrix());
            shaderPBR->setMat3("normalMatrix", glm::transpose(glm::inverse(object->getModelMatrix())));

            object->draw(shaderPBR.get());
        }
    frameBufferMSAA->unbind();
}

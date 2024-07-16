#include "../../Renderer.h"

void Renderer::renderPBR(FrameBuffer* frameBuffer) {
    frameBuffer->bind();
    frameBuffer->reset();
        shaderPBR->use();
//        shaderPBR->setInt("normalMap", 1);
//        shaderPBR->setInt("metallicMap", 2);
//        shaderPBR->setInt("roughnessMap", 3);
//        shaderPBR->setInt("aoMap", 4);

        shaderPBR->setTexture("irradianceMap", scene.skybox->irradiance.get());
        shaderPBR->setTexture("prefilterMap", scene.skybox->prefilter.get());
        shaderPBR->setTexture("brdfLUT", scene.skybox->brdfLUT.get());
        for (int i = 0; i < scene.lights.size(); ++i)
        {
            shaderPBR->setVec3("lightPositions[" + std::to_string(i) + "]", scene.lights[i]->position);
            shaderPBR->setVec3("lightColors[" + std::to_string(i) + "]", scene.lights[i]->getColor());
        }
        shaderPBR->setVec3("camPos", scene.camera->Position);
        shaderPBR->setMat4("view", scene.camera->getViewMatrix());
        shaderPBR->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        for (auto& object : scene.objects) {
            shaderPBR->setMat4("model", object->getModelMatrix());
            shaderPBR->setMat3("normalMatrix", glm::transpose(glm::inverse(object->getModelMatrix())));

            object->draw(shaderPBR.get());
        }
    frameBuffer->unbind();
}

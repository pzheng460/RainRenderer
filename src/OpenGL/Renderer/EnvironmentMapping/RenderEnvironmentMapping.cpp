#include "../../Renderer.h"

void Renderer::renderEnvironmentMapping() {
    frameBufferDefault->bind();
    frameBufferDefault->reset();
        shaderEnvironmentMapping->use();

        shaderEnvironmentMapping->setInt("skybox", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, scene.skybox->getCubemapTexture());

//        shaderEnvironmentMapping->setTexture("skybox", scene.skybox.getTexture());
        shaderEnvironmentMapping->setVec3("cameraPos", scene.camera->Position);

        shaderEnvironmentMapping->setMat4("view", scene.camera->getViewMatrix());
        shaderEnvironmentMapping->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        for (auto& object : scene.objects) {
            shaderEnvironmentMapping->setMat4("model", object->getModelMatrix());

            object->draw();
        }
    frameBufferDefault->unbind();
}
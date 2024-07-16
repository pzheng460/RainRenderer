#include "../../Renderer.h"

void Renderer::renderSkybox(FrameBuffer *frameBuffer) {
    frameBuffer->bind();
    glDepthFunc(GL_LEQUAL);
    shaderSkybox->use();

    shaderSkybox->setTexture("skybox", scene.skybox->cubeMap.get());
    shaderSkybox->setMat4("view", glm::mat4(glm::mat3(scene.camera->getViewMatrix()))); // remove translation from the view matrix);
    shaderSkybox->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

    scene.skybox->draw();
    glDepthFunc(GL_LESS);
    frameBuffer->unbind();
}

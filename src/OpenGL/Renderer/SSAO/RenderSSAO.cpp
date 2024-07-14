#include "../../Renderer.h"

void Renderer::renderSSAO() {
    frameBufferSSAO->bind();
    frameBufferSSAO->reset();
        shaderSSAO->use();

        shaderSSAO->setTexture("gPosition", frameBufferIntermediate->textureColorBuffers[0].get());
        shaderSSAO->setTexture("gNormal", frameBufferIntermediate->textureColorBuffers[1].get());
        shaderSSAO->setTexture("texNoise", ssao.noiseTexture.get());
        shaderSSAO->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        // Send kernel + rotation
        for (unsigned int i = 0; i < 64; ++i)
            shaderSSAO->setVec3("samples[" + std::to_string(i) + "]", ssao.ssaoKernel[i]);

        screenQuad->draw();
    frameBufferSSAO->unbind();
}

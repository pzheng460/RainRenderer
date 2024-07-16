#include "../../Renderer.h"

void Renderer::renderSSAO(FrameBuffer* frameBuffer) {
    frameBuffer->bind();
    frameBuffer->reset();
        shaderSSAO->use();

        shaderSSAO->setTexture("gPosition", frameBufferGeometry->textureColorBuffers[0].get());
        shaderSSAO->setTexture("gNormal", frameBufferGeometry->textureColorBuffers[1].get());
        shaderSSAO->setTexture("texNoise", ssao.noiseTexture.get());
        shaderSSAO->setMat4("projection", scene.camera->getProjectionMatrix(width, height));

        // Send kernel + rotation
        for (unsigned int i = 0; i < 64; ++i)
            shaderSSAO->setVec3("samples[" + std::to_string(i) + "]", ssao.ssaoKernel[i]);

        screenQuad->draw();
    frameBuffer->unbind();
}

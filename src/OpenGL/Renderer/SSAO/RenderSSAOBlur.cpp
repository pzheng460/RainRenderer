#include "../../Renderer.h"

void Renderer::renderSSAOBlur() {
    frameBufferSSAOBlur->bind();
    frameBufferSSAOBlur->reset();
        shaderSSAOBlur->use();

        shaderSSAOBlur->setTexture("ssaoInput", frameBufferSSAO->textureColorBuffers[0].get());

        screenQuad->draw();
    frameBufferSSAOBlur->unbind();
}

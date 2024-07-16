#include "../../Renderer.h"

void Renderer::renderSSAOBlur(FrameBuffer* frameBuffer) {
    frameBuffer->bind();
    frameBuffer->reset();
        shaderSSAOBlur->use();

        shaderSSAOBlur->setTexture("ssaoInput", frameBufferSSAO->textureColorBuffers[0].get());

        screenQuad->draw();
    frameBuffer->unbind();
}

#include "../../Renderer.h"

void Renderer::renderBloom(FrameBuffer* frameBuffer0, FrameBuffer* frameBuffer1) {
    frameBuffer0->bind();
    shaderBloom->use();
    shaderBloom->setTexture("image", first_iteration ? frameBufferIntermediate->textureColorBuffers[1].get() : frameBuffer1->textureColorBuffers[0].get());
    shaderBloom->setInt("horizontal", horizontal);

    screenQuad->draw();

    horizontal = !horizontal;
    if (first_iteration)
        first_iteration = false;
    frameBuffer0->unbind();
}

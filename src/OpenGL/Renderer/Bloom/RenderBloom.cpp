#include "../../Renderer.h"

void Renderer::renderBloom() {
    bool first_iteration = true;

    for (unsigned int i = 0; i < amount; ++i)
    {
        frameBufferBlooms[horizontal]->bind();
            shaderBloom->use();
            shaderBloom->setTexture("image", first_iteration ? frameBufferIntermediate->textureColorBuffers[1].get() : frameBufferBlooms[!horizontal]->textureColorBuffers[0].get());
            shaderBloom->setInt("horizontal", horizontal);

            screenQuad->draw();

            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        frameBufferBlooms[horizontal]->unbind();
    }
}

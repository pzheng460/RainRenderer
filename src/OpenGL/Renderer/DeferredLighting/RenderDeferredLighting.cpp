#include "../../Renderer.h"

void Renderer::renderDeferredLighting() {
    frameBufferIntermediate->bind();
    frameBufferIntermediate->reset();
        shaderDeferredLighting->use();
        shaderDeferredLighting->setTexture("gPosition", frameBufferGeometry->textureColorBuffers[0].get());
        shaderDeferredLighting->setTexture("gNormal", frameBufferGeometry->textureColorBuffers[1].get());
        shaderDeferredLighting->setTexture("gAlbedoSpec", frameBufferGeometry->textureColorBuffers[2].get());
        shaderDeferredLighting->setTexture("ssao", frameBufferSSAOBlur->textureColorBuffers[0].get());

        for (unsigned int i = 0; i < scene.lights.size(); ++i)
        {
            shaderDeferredLighting->setVec3("lights[" + std::to_string(i) + "].Position", scene.lights[i]->position);
            shaderDeferredLighting->setVec3("lights[" + std::to_string(i) + "].Color", scene.lights[i]->getColor());
            shaderDeferredLighting->setFloat("lights[" + std::to_string(i) + "].Linear", scene.lights[i]->getLinear());
            shaderDeferredLighting->setFloat("lights[" + std::to_string(i) + "].Quadratic", scene.lights[i]->getQuadratic());
            // then calculate radius of light volume/sphere
            const float maxBrightness = std::fmaxf(std::fmaxf(scene.lights[i]->getColor().r, scene.lights[i]->getColor().g), scene.lights[i]->getColor().b);
            float radius = (-scene.lights[i]->getLinear() + std::sqrt(scene.lights[i]->getLinear() * scene.lights[i]->getLinear() - 4 * scene.lights[i]->getQuadratic() * (scene.lights[i]->getConstant() - (256.0f / 5.0f) * maxBrightness))) / (2.0f * scene.lights[i]->getQuadratic());
            shaderDeferredLighting->setFloat("lights[" + std::to_string(i) + "].Radius", radius);
        }
        shaderDeferredLighting->setVec3("viewPos", scene.camera->Position);
        shaderDeferredLighting->setInt("size", static_cast<int>(scene.lights.size()));

        shaderDeferredLighting->setBool("ssaoActive", gui.SSAOActive);

        screenQuad->draw();
    frameBufferIntermediate->unbind();
}
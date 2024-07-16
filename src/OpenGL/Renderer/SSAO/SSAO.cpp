#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SSAO.h"

SSAO::SSAO() {
    init();
}

void SSAO::init() {
    generateSampleKernel();
    generateSSAONoise();

    Texture* noiseTexturePtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_SSAO_NOISE);
    noiseTexturePtr->setSize(4, 4);
    dynamic_cast<Texture2D*>(noiseTexturePtr)->init(&ssaoNoise[0]);
    noiseTexture = std::shared_ptr<Texture>(noiseTexturePtr);
}

void SSAO::generateSampleKernel() {
    for (unsigned int i = 0; i < 64; ++i) {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0f;

        // scale samples s.t. they're more aligned to center of kernel
        scale = ourLerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }
}

void SSAO::generateSSAONoise() {
    for (unsigned int i = 0; i < 16; i++) {
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        ssaoNoise.push_back(noise);
    }
}
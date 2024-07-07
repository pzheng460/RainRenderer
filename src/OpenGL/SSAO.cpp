#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SSAO.h"
#include "shaderSetting.h"

SSAO::SSAO(Camera& camera, int width, int height, FrameBuffer& gFrameBuffer) :
    camera(camera),
    width(width),
    height(height),
    gFrameBuffer(gFrameBuffer) {
    init();
}

void SSAO::init() {
    ssaoFrameBuffer.generateFrameBuffer(width, height);
    ssaoBlurFrameBuffer.generateFrameBuffer(width, height);

    generateSampleKernel();
    generateSSAONoise();

    Texture* noiseTexturePtr = TextureFactory::createTexture(TextureFactoryType::TEXTURE_SSAO_NOISE);
    noiseTexture = std::shared_ptr<Texture>(noiseTexturePtr);
    noiseTexture->generateTexture(4, 4, &ssaoNoise[0]);
}

void SSAO::reset(int newWidth, int newHeight) {
    this->width = newWidth;
    this->height = newHeight;
    init();
}

unsigned int SSAO::draw() {
    // generate SSAO texture 生成SSAO纹理
    ssaoFrameBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ssaoShaderSetting();
    Model screenQuadModel(GeometryType::QUAD);
    Object screenQuad(screenQuadModel);
    screenQuad.draw(ssaoShader);
    ssaoFrameBuffer.unbind();

    // generate SSAO blur texture to remove noise 生成SSAO模糊纹理以去除噪声
    ssaoBlurFrameBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoBlurShaderSetting();
    screenQuad.draw(ssaoBlurShader);
    ssaoBlurFrameBuffer.unbind();

    return ssaoBlurFrameBuffer.textureColorBuffers[0]->textureID;
}

void SSAO::ssaoShaderSetting() {
    ssaoShader.use();
    ssaoShader.setInt("gPosition", 0);
    ssaoShader.setInt("gNormal", 1);
    ssaoShader.setInt("texNoise", 2);

    // Send kernel + rotation
    for (unsigned int i = 0; i < 64; ++i)
        ssaoShader.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
    auto projectionMatrix = glm::perspective(glm::radians(camera.Zoom), float(width / height), 0.1f, 100.0f);
    ssaoShader.setMat4("projection", projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(gFrameBuffer.textureColorBuffers[0]->target, gFrameBuffer.textureColorBuffers[0]->textureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(gFrameBuffer.textureColorBuffers[0]->target, gFrameBuffer.textureColorBuffers[1]->textureID);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(gFrameBuffer.textureColorBuffers[0]->target, noiseTexture->textureID);
}

void SSAO::ssaoBlurShaderSetting() {
    ssaoBlurShader.use();
    ssaoBlurShader.setInt("ssaoInput", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(ssaoFrameBuffer.textureColorBuffers[0]->target, ssaoFrameBuffer.textureColorBuffers[0]->textureID);
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
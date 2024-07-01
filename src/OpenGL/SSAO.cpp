#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SSAO.h"
#include "render_implicit_geometry.h"
#include "shaderSetting.h"

void NoiseTexture::generateNoiseTexture(std::vector<glm::vec3>& ssaoNoise) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void SSAOTexture::generateTexture(int SCR_WIDTH, int SCR_HEIGHT) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

SSAOFrameBuffer::SSAOFrameBuffer(int numOfColorAttachments, int numOfDepthAttachments) :
    FrameBuffer(numOfColorAttachments, numOfDepthAttachments) {
    for (int i = 0; i < numOfColorAttachments; i++) {
        std::unique_ptr<SSAOTexture> textureColorBuffer = std::make_unique<SSAOTexture>();
        textureColorBuffers[i] = std::move(textureColorBuffer);
    }
}

SSAO::SSAO(Camera& camera, int SCR_WIDTH, int SCR_HEIGHT, GeometryFrameBuffer& gFrameBuffer) :
    camera(camera),
    SCR_WIDTH(SCR_WIDTH),
    SCR_HEIGHT(SCR_HEIGHT),
    gFrameBuffer(gFrameBuffer) {
    init();
}

void SSAO::init() {
    ssaoFrameBuffer.generateFrameBuffer(SCR_WIDTH, SCR_HEIGHT);
    ssaoBlurFrameBuffer.generateFrameBuffer(SCR_WIDTH, SCR_HEIGHT);

    generateSampleKernel();
    generateSSAONoise();
    noiseTexture.generateNoiseTexture(ssaoNoise);
}

unsigned int SSAO::draw() {
    // generate SSAO texture 生成SSAO纹理
    ssaoFrameBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ssaoShaderSetting();
    renderQuad();
    ssaoFrameBuffer.unbind();

    // generate SSAO blur texture to remove noise 生成SSAO模糊纹理以去除噪声
    ssaoBlurFrameBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoBlurShaderSetting();
    renderQuad();
    ssaoBlurFrameBuffer.unbind();

    return ssaoBlurFrameBuffer.getTextureColorBuffer()[0]->getTexture();
}

void SSAO::ssaoShaderSetting() {
    ssaoShader.use();
    ssaoShader.setInt("gPosition", 0);
    ssaoShader.setInt("gNormal", 1);
    ssaoShader.setInt("texNoise", 2);

    // Send kernel + rotation
    for (unsigned int i = 0; i < 64; ++i)
        ssaoShader.setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
    auto projectionMatrix = glm::perspective(glm::radians(camera.Zoom), float(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
    ssaoShader.setMat4("projection", projectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gFrameBuffer.getTextureColorBuffer()[0]->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gFrameBuffer.getTextureColorBuffer()[1]->getTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, noiseTexture.getTexture());
}

void SSAO::ssaoBlurShaderSetting() {
    ssaoBlurShader.use();
    ssaoBlurShader.setInt("ssaoInput", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssaoFrameBuffer.getTextureColorBuffer()[0]->getTexture());
}


void SSAO::generateSampleKernel() {
    for (unsigned int i = 0; i < 64; ++i)
    {
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
    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        ssaoNoise.push_back(noise);
    }
}
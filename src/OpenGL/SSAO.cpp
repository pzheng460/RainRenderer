#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SSAO.h"
#include "shaderSetting.h"

void NoiseTexture::specifyTexture(GLvoid *data) {
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
}

void SSAOColorTexture::specifyTexture(GLvoid *data) {
    glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, data);
}

SSAOFrameBuffer::SSAOFrameBuffer(int numOfColorTextureAttachments, int numOfDepthTextureAttachments, int numOfRenderBufferObjectDepth) :
    FrameBuffer(numOfColorTextureAttachments, numOfDepthTextureAttachments, numOfRenderBufferObjectDepth) {
    for (int i = 0; i < numOfColorTextureAttachments; i++) {
        auto textureColorBuffer = std::make_unique<SSAOColorTexture>();
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
    noiseTexture.generateTexture(4, 4, &ssaoNoise[0]);
}

void SSAO::reset(int SCR_WIDTH, int SCR_HEIGHT) {
    this->SCR_WIDTH = SCR_WIDTH;
    this->SCR_HEIGHT = SCR_HEIGHT;
    init();
}

unsigned int SSAO::draw() {
    // generate SSAO texture 生成SSAO纹理
    ssaoFrameBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ssaoShaderSetting();
    Object screenQuad(QUAD);
    screenQuad.draw(ssaoShader);
    ssaoFrameBuffer.unbind();

    // generate SSAO blur texture to remove noise 生成SSAO模糊纹理以去除噪声
    ssaoBlurFrameBuffer.bind();
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoBlurShaderSetting();
    screenQuad.draw(ssaoBlurShader);
    ssaoBlurFrameBuffer.unbind();

    return ssaoBlurFrameBuffer.getTextureColorBuffer()[0]->textureID;
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
    glBindTexture(gFrameBuffer.getTextureColorBuffer()[0]->target, gFrameBuffer.getTextureColorBuffer()[0]->textureID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(gFrameBuffer.getTextureColorBuffer()[0]->target, gFrameBuffer.getTextureColorBuffer()[1]->textureID);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(gFrameBuffer.getTextureColorBuffer()[0]->target, noiseTexture.textureID);
}

void SSAO::ssaoBlurShaderSetting() {
    ssaoBlurShader.use();
    ssaoBlurShader.setInt("ssaoInput", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(ssaoFrameBuffer.getTextureColorBuffer()[0]->target, ssaoFrameBuffer.getTextureColorBuffer()[0]->textureID);
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
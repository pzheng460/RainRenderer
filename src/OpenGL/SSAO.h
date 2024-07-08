#ifndef SSAO_H
#define SSAO_H

#pragma once

#include <utils/filesystem.h>
#include <random>

#include "Shader.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "Object.h"

class SSAO {
public:
    SSAO(Camera& camera, int width, int height, FrameBuffer& gFrameBuffer);
    void init();
    void reset(int newWidth, int newHeight);
    unsigned int draw();

    void generateSampleKernel();
    void generateSSAONoise();

    void ssaoShaderSetting();
    void ssaoBlurShaderSetting();

    FrameBuffer& getSSAOBlurFrameBuffer() {
        return ssaoBlurFrameBuffer;
    }
private:
    int width, height;
    Camera& camera;
    FrameBuffer& gFrameBuffer;
    Shader ssaoShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_SSAO);
    Shader ssaoBlurShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_SSAO_BLUR);
    FrameBuffer ssaoFrameBuffer = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_SSAO);
    FrameBuffer ssaoBlurFrameBuffer = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_SSAO_BLUR);
    std::uniform_real_distribution<GLfloat> randomFloats = std::uniform_real_distribution<GLfloat>(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    std::vector<glm::vec3> ssaoKernel;
    std::vector<glm::vec3> ssaoNoise;
    std::shared_ptr<Texture> noiseTexture;
};

inline float ourLerp(float a, float b, float f)
{
    return a + f * (b - a);
}

#endif // SSAO_H

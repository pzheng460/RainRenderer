#ifndef SSAO_H
#define SSAO_H

#include <utils/shader.h>
#include <utils/filesystem.h>
#include "FrameBuffer.h"
#include "GeometryFrameBuffer.h"
#include <random>
#include <utils/camera.h>

class NoiseTexture : public Textures {
public:
    NoiseTexture() = default;
    void generateNoiseTexture(std::vector<glm::vec3>& ssaoNoise);
};

class SSAOTexture : public Textures {
public:
    SSAOTexture() = default;
    void generateTexture(int SCR_WIDTH, int SCR_HEIGHT) override;
};

class SSAOFrameBuffer : public FrameBuffer {
public:
    SSAOFrameBuffer(int numOfColorAttachments = 1, int numOfDepthAttachments = 0);
};

class SSAO {
public:
    SSAO(Camera& camera, int SCR_WIDTH, int SCR_HEIGHT, GeometryFrameBuffer& gFrameBuffer);
    void init();
    unsigned int draw();

    void generateSampleKernel();
    void generateSSAONoise();

    void ssaoShaderSetting();
    void ssaoBlurShaderSetting();
private:
    int SCR_WIDTH, SCR_HEIGHT;
    Camera& camera;
    GeometryFrameBuffer& gFrameBuffer;
    Shader ssaoShader = Shader(FileSystem::getPath("src/OpenGL/shaders/ssao.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/ssao.fs").c_str());
    Shader ssaoBlurShader = Shader(FileSystem::getPath("src/OpenGL/shaders/ssao.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/ssao_blur.fs").c_str());
    SSAOFrameBuffer ssaoFrameBuffer = SSAOFrameBuffer(1);
    SSAOFrameBuffer ssaoBlurFrameBuffer = SSAOFrameBuffer(1);
    std::uniform_real_distribution<GLfloat> randomFloats = std::uniform_real_distribution<GLfloat>(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    std::vector<glm::vec3> ssaoKernel;
    std::vector<glm::vec3> ssaoNoise;
    NoiseTexture noiseTexture;
};

inline float ourLerp(float a, float b, float f)
{
    return a + f * (b - a);
}

#endif // SSAO_H

#ifndef SSAO_H
#define SSAO_H

#include <utils/filesystem.h>
#include <random>

#include "Shader.h"
#include "Camera.h"
#include "FrameBuffer.h"
#include "GeometryFrameBuffer.h"
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
private:
    int width, height;
    Camera& camera;
    FrameBuffer& gFrameBuffer;
    Shader ssaoShader = Shader(FileSystem::getPath("src/OpenGL/shaders/ssao.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/ssao.fs").c_str());
    Shader ssaoBlurShader = Shader(FileSystem::getPath("src/OpenGL/shaders/ssao.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/ssao_blur.fs").c_str());
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

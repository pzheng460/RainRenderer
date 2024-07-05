#ifndef SSAO_H
#define SSAO_H

#include <utils/shader.h>
#include <utils/filesystem.h>
#include <random>
#include <utils/camera.h>
#include "FrameBuffer.h"
#include "GeometryFrameBuffer.h"
#include "Object.h"

class NoiseTexture : public Texture {
public:
    NoiseTexture() {
        target = GL_TEXTURE_2D;
        internalFormat = GL_RGB16F;
        format = GL_RGB;
        type = GL_FLOAT;
    }
    void specifyTexture(GLvoid* data) override;
};

class SSAOColorTexture : public Texture {
public:
    SSAOColorTexture() {
        target = GL_TEXTURE_2D;
        internalFormat = GL_RED;
        format = GL_RED;
        type = GL_FLOAT;

        minFilter = GL_NEAREST;
        magFilter = GL_NEAREST;
        wrapS = GL_CLAMP_TO_EDGE;
        wrapT = GL_CLAMP_TO_EDGE;
    }
    void specifyTexture(GLvoid* data) override;
};

class SSAOFrameBuffer : public FrameBuffer {
public:
    SSAOFrameBuffer(int numOfColorTextureAttachments = 1, int numOfDepthTextureAttachments = 0, int numOfRenderBufferObjectDepth = 0);
};

class SSAO {
public:
    SSAO(Camera& camera, int SCR_WIDTH, int SCR_HEIGHT, GeometryFrameBuffer& gFrameBuffer);
    void init();
    void reset(int SCR_WIDTH, int SCR_HEIGHT);
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

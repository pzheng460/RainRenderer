#ifndef SSAO_H
#define SSAO_H

#pragma once

#include <utils/filesystem.h>
#include <random>

#include "../../Texture.h"

class SSAO {
public:
    SSAO();
    void init();

    void generateSampleKernel();
    void generateSSAONoise();

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

#ifndef SHADERSETTING_H
#define SHADERSETTING_H

#pragma once

#include <utils/shader.h>
#include <utils/light.h>
#include <vector>
#include "FrameBuffer.h"

inline void phongShaderSetting(Shader& shader, const Camera& camera, std::vector<Light>& lights, std::vector<FrameBuffer>& shadowMapFrameBuffers, bool shadowActive = false) {
    shader.use();
    if (shadowActive) {
        shader.setBool("shadowActive", shadowActive);
    }

    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        if (shadowActive) {
            shader.setInt("shadowMaps[" + std::to_string(i) + "]", 4 + i);
            shader.setMat4("lightSpaceMatrix[" + std::to_string(i) + "]", lights[i].getLightSpaceMatrix());
        }
        shader.setVec3("pointLights[" + std::to_string(i) + "].position", lights[i].getPosition());
        shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", lights[i].getAmbientColor());
        shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", lights[i].getDiffuseColor());
        shader.setVec3("pointLights[" + std::to_string(i) + "].specular", lights[i].getSpecularColor());

        shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
        shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
        shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
    }

    shader.setVec3("viewPos", camera.Position);
    shader.setFloat("material.shininess", 64.0f);
    shader.setInt("size", lights.size());

    if (shadowActive) {
        for (unsigned int i = 0; i < shadowMapFrameBuffers.size(); ++i) {
            glActiveTexture(GL_TEXTURE4 + i);
            glBindTexture(GL_TEXTURE_2D, shadowMapFrameBuffers[i].getTextureDepthBuffer()->getTexture());
        }
    }
}

inline void shadowMapShaderSetting(Shader& shader, const Light& light) {
    shader.use();
    shader.setMat4("lightSpaceMatrix", light.getLightSpaceMatrix());
}

inline void PBRShaderSetting(Shader& shader, std::vector<Light> lights, unsigned int irradianceMap, unsigned int prefilterMap, unsigned int brdfLUTTexture) {
    shader.use();
    shader.setInt("albedoMap", 0);
    shader.setInt("normalMap", 1);
    shader.setInt("metallicMap", 2);
    shader.setInt("roughnessMap", 3);
    shader.setInt("aoMap", 4);
    shader.setInt("irradianceMap", 5);
    shader.setInt("prefilterMap", 6);
    shader.setInt("brdfLUT", 7);

    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        shader.setVec3("lightPositions[" + std::to_string(i) + "]", lights[i].getPosition());
        shader.setVec3("lightColors[" + std::to_string(i) + "]", lights[i].getColor());
    }

    // bind pre-computed IBL data
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
}

inline void debugDepthQuadShaderSetting(Shader& shader, unsigned int depthMap) {
    float near_plane = 1.0f, far_plane = 100.0f;
    shader.use();
    shader.setFloat("near_plane", near_plane);
    shader.setFloat("far_plane", far_plane);
    shader.setInt("depthMap", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}

inline void finalShaderSetting(Shader& shader, unsigned int hdrBuffer, bool HDRActive, float exposure, unsigned int bloomBlur, bool bloomActive) {
    shader.use();

    shader.setInt("hdrBuffer", 0);
    shader.setBool("HDRActive", HDRActive);
    shader.setFloat("exposure", exposure);

    shader.setInt("bloomBlur", 1);
    shader.setBool("bloomActive", bloomActive);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrBuffer);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, bloomBlur);
}

inline void geometryBufferShaderSetting(Shader& shader, bool invertNormals) {
    shader.use();
    shader.setBool("invertNormals", invertNormals);
}

inline void deferredLightingShaderSetting(Shader& shader, unsigned int gPosition, unsigned int gNormal, unsigned int gAlbedoSpec, unsigned int ssaoColorBuffer, Camera& camera, std::vector<Light> lights, bool ssaoActive) {
    shader.use();
    shader.setInt("gPosition", 0);
    shader.setInt("gNormal", 1);
    shader.setInt("gAlbedoSpec", 2);
    shader.setInt("ssao", 3);

    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        shader.setVec3("lights[" + std::to_string(i) + "].Position", lights[i].getPosition());
        shader.setVec3("lights[" + std::to_string(i) + "].Color", lights[i].getColor());
        shader.setFloat("lights[" + std::to_string(i) + "].Linear", lights[i].getLinear());
        shader.setFloat("lights[" + std::to_string(i) + "].Quadratic", lights[i].getQuadratic());
        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(lights[i].getColor().r, lights[i].getColor().g), lights[i].getColor().b);
        float radius = (-lights[i].getLinear() + std::sqrt(lights[i].getLinear() * lights[i].getLinear() - 4 * lights[i].getQuadratic() * (lights[i].getConstant() - (256.0f / 5.0f) * maxBrightness))) / (2.0f * lights[i].getQuadratic());
        shader.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
    }
    shader.setVec3("viewPos", camera.Position);
    shader.setInt("size", lights.size());

    shader.setBool("ssaoActive", ssaoActive);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
}

#endif // SHADERSETTING_H

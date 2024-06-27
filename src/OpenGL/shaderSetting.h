#ifndef SHADERSETTING_H
#define SHADERSETTING_H

#pragma once

#include <learnopengl/shader.h>
#include <learnopengl/light.h>
#include <vector>
#include "ShadowMap.h"

inline void phongShaderSetting(Shader& shader, const Camera& camera, std::vector<Light>& lights, std::vector<ShadowMap>& shadowMaps, bool shadowActive = false) {
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
        for (unsigned int i = 0; i < shadowMaps.size(); ++i) {
            glActiveTexture(GL_TEXTURE4 + i);
            glBindTexture(GL_TEXTURE_2D, shadowMaps[i].getDepthMap());
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

inline void HDRShaderSetting(Shader& shader, unsigned int colorBuffer, bool HDRActive, float exposure = 1.0f) {
    shader.use();
    shader.setInt("hdrBuffer", 0);
    shader.setBool("HDRActive", HDRActive);
    shader.setFloat("exposure", exposure);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
}
#endif // SHADERSETTING_H

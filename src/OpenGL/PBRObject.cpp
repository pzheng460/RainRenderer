#include "PBRObject.h"
#include <iostream>
#include "render_implicit_geometry.h"

PBRObject::PBRObject(const Model& model, const Shader& shader)
        : Object(model, shader) {
    loadPBRTextures();
}

PBRObject::PBRObject(ImplicitGeometryType geometryType, const Shader& shader)
        : Object(geometryType, shader) {
    loadPBRTextures();
}

void PBRObject::loadPBRTextures() {
    // Load PBR textures
    albedoMap = loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/albedo.png").c_str());
    normalMap = loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/normal.png").c_str());
    metallicMap = loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/metallic.png").c_str());
    roughnessMap = loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/roughness.png").c_str());
    aoMap = loadTexture(FileSystem::getPath("resources/textures/pbr/rusted_iron/ao.png").c_str());
}

void PBRObject::PBRShaderSetting(std::vector<Light> &lights, unsigned int irradianceMap, unsigned int prefilterMap,
                                 unsigned int brdfLUTTexture) {
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

    // MVP matrices
    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);
    shader.setMat3("normalMatrix", glm::transpose(glm::inverse(modelMatrix)));

    // bind pre-computed PBR data
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedoMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normalMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallicMap);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughnessMap);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, albedoMap);

    // bind pre-computed IBL data
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
}
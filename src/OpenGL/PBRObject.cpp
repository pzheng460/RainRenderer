#include "PBRObject.h"
#include <iostream>
#include "render_implicit_geometry.h"

PBRObject::PBRObject(const Model& model, const Shader& shader)
        : Object(model, shader) {
//    loadPBRTextures();
}

PBRObject::PBRObject(ImplicitGeometryType geometryType, const Shader& shader)
        : Object(geometryType, shader, false) {
    loadPBRTextures(model.meshes[0].textures,
                    FileSystem::getPath("resources/textures/pbr/rusted_iron/albedo.png"),
                    FileSystem::getPath("resources/textures/pbr/rusted_iron/normal.png"),
                    FileSystem::getPath("resources/textures/pbr/rusted_iron/metallic.png"),
                    FileSystem::getPath("resources/textures/pbr/rusted_iron/roughness.png"),
                    FileSystem::getPath("resources/textures/pbr/rusted_iron/ao.png"));
}

void PBRObject::loadPBRTextures(std::vector<Texture>& textures, const std::string& albedoPath, const std::string& normalPath, const std::string& metallicPath, const std::string& roughnessPath, const std::string& aoPath) {
    // Load PBR textures
    textures.push_back(loadTexture(albedoPath, "albedoMap"));
    textures.push_back(loadTexture(normalPath, "normalMap"));
    textures.push_back(loadTexture(metallicPath, "metallicMap"));
    textures.push_back(loadTexture(roughnessPath, "roughnessMap"));
    textures.push_back(loadTexture(aoPath, "aoMap"));
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

    // bind pre-computed IBL data
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
}

void PBRObject::draw() {
    model.Draw(shader, GL_TRIANGLE_STRIP);
}
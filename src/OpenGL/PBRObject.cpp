#include "PBRObject.h"
#include <iostream>
#include "render_implicit_geometry.h"

PBRObject::PBRObject(const AssimpModel::Model& model)
        : Object(model) {
//    loadPBRTextures();
}

PBRObject::PBRObject(ImplicitGeometryType geometryType)
        : Object(geometryType, false) {
    loadPBRTextures(model.meshes[0].textures,
                    FileSystem::getPath("resources/textures/pbr/rusted_iron/albedo.png"),
                    FileSystem::getPath("resources/textures/pbr/rusted_iron/normal.png"),
                    FileSystem::getPath("resources/textures/pbr/rusted_iron/metallic.png"),
                    FileSystem::getPath("resources/textures/pbr/rusted_iron/roughness.png"),
                    FileSystem::getPath("resources/textures/pbr/rusted_iron/ao.png"));
}

void PBRObject::loadPBRTextures(std::vector<AssimpModel::Texture>& textures, const std::string& albedoPath, const std::string& normalPath, const std::string& metallicPath, const std::string& roughnessPath, const std::string& aoPath) {
    // Load PBR textures
    textures.push_back(loadTexture(albedoPath, "albedoMap"));
    textures.push_back(loadTexture(normalPath, "normalMap"));
    textures.push_back(loadTexture(metallicPath, "metallicMap"));
    textures.push_back(loadTexture(roughnessPath, "roughnessMap"));
    textures.push_back(loadTexture(aoPath, "aoMap"));
}

void PBRObject::draw(Shader& shader) {
    shader.use();
    // MVP matrices
    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);
    shader.setMat3("normalMatrix", glm::transpose(glm::inverse(modelMatrix)));
    model.Draw(shader, GL_TRIANGLE_STRIP);
}
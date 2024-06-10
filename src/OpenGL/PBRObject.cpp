#include "PBRObject.h"
#include <iostream>
#include "render_implicit_geometry.h"

PBRObject::PBRObject(const Model& model)
        : Object(model, RenderMode::PBR) {
    loadPBRTextures();
}

PBRObject::PBRObject(ImplicitGeometryType geometryType)
        : Object(geometryType, RenderMode::PBR) {
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

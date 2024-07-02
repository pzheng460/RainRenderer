#ifndef PBROBJECT_H
#define PBROBJECT_H

#include "Object.h"
#include <utils/filesystem.h>
#include <utils/light.h>

class PBRObject : public Object {
public:
    PBRObject(const AssimpModel::Model& model, const Shader& shader);
    PBRObject(ImplicitGeometryType geometryType, const Shader& shader);

    void draw() override;

private:
    void loadPBRTextures(std::vector<AssimpModel::Texture>& textures, const std::string& albedoPath, const std::string& normalPath, const std::string& metallicPath, const std::string& roughnessPath, const std::string& aoPath);
};

#endif // PBROBJECT_H

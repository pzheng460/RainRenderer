#ifndef PBROBJECT_H
#define PBROBJECT_H

#include "Object.h"
#include <learnopengl/filesystem.h>
#include <learnopengl/light.h>

class PBRObject : public Object {
public:
    PBRObject(const Model& model, const Shader& shader);
    PBRObject(ImplicitGeometryType geometryType, const Shader& shader);

    void PBRShaderSetting(std::vector<Light>& lights, unsigned int irradianceMap, unsigned int prefilterMap, unsigned int brdfLUTTexture);
    void draw() override;

private:
    void loadPBRTextures(std::vector<Texture>& textures, const std::string& albedoPath, const std::string& normalPath, const std::string& metallicPath, const std::string& roughnessPath, const std::string& aoPath);
};

#endif // PBROBJECT_H

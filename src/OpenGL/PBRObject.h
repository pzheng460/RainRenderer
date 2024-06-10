#ifndef PBROBJECT_H
#define PBROBJECT_H

#include "Object.h"
#include <learnopengl/filesystem.h>

class PBRObject : public Object {
public:
    PBRObject(const Model& model);
    PBRObject(ImplicitGeometryType geometryType);

    void loadPBRTextures();

    unsigned int getAlbedoMap() const override { return albedoMap; }
    unsigned int getNormalMap() const override { return normalMap; }
    unsigned int getMetallicMap() const override { return metallicMap; }
    unsigned int getRoughnessMap() const override { return roughnessMap; }
    unsigned int getAOMap() const override { return aoMap; }

private:
    unsigned int albedoMap;
    unsigned int normalMap;
    unsigned int metallicMap;
    unsigned int roughnessMap;
    unsigned int aoMap;
};

#endif // PBROBJECT_H

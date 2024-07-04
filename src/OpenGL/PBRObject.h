#ifndef PBROBJECT_H
#define PBROBJECT_H

#include "Object.h"
#include <utils/filesystem.h>
#include <utils/light.h>

class PBRObject : public Object {
public:
    PBRObject(const AssimpModel::Model& model);
    PBRObject(GeometryType geometryType);

    void draw(Shader& shader, bool setTexture = false);
};

#endif // PBROBJECT_H

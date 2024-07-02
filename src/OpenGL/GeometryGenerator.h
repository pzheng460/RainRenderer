#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include <vector>
#include <string>
#include <utils/model.h>

class GeometryGenerator {
public:
    static AssimpModel::Model generateSphere();
    static AssimpModel::Model generateCube();
    static AssimpModel::Model generateQuad();
    static AssimpModel::Model generatePlane();
private:
    GeometryGenerator() = default;
};

#endif // GEOMETRYGENERATOR_H

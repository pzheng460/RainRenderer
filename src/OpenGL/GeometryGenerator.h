#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include <vector>
#include <string>
#include <learnopengl/model.h>

class GeometryGenerator {
public:
    static Model generateSphere();
    static Model generateCube();
    static Model generateQuad();
    static Model generatePlane();
private:
    GeometryGenerator() = default;
};

#endif // GEOMETRYGENERATOR_H

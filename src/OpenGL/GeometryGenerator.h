#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include <vector>
#include <string>
#include <learnopengl/model.h>

class GeometryGenerator {
public:
    static Model generateSphere(const std::string& albedoPath, const std::string& normalPath, const std::string& metallicPath, const std::string& roughnessPath, const std::string& aoPath);
    static Model generateCube(const std::string& albedoPath, const std::string& normalPath, const std::string& metallicPath, const std::string& roughnessPath, const std::string& aoPath);
    static Model generateQuad(const std::string& albedoPath, const std::string& normalPath, const std::string& metallicPath, const std::string& roughnessPath, const std::string& aoPath);

private:
    static Texture loadTexture(const std::string& path, const std::string& typeName);
};

#endif // GEOMETRYGENERATOR_H

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <utils/filesystem.h>
#include "Camera.h"
#include "Shader.h"
#include "Object.h"

const glm::vec3 AMBIENT = glm::vec3(0.5);
const glm::vec3 DIFFUSE = glm::vec3(0.5);
const glm::vec3 SPECULAR = glm::vec3(1.0);

class Light : public Object {
public:
    Light(const glm::vec3& position, const glm::vec3& color)
            : color(color)
            , ambient(AMBIENT)
            , diffuse(DIFFUSE)
            , specular(SPECULAR) {
        updateLightColor();
        updateLightSpaceMatrix();
        this->position = position;
        model = Model(GeometryType::CUBE);
    }

    void updateLightColor() {
        ambientColor = color * ambient;
        diffuseColor = color * diffuse;
        specularColor = color * specular;
    }

    void updateLightSpaceMatrix() {
        lightProjection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, near_plane, far_plane);
        lightView = glm::lookAt(position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
    }

    void setColor(const glm::vec3& newColor) {
        this->color = newColor;
        updateLightColor();
    }

    const glm::vec3& getColor() const { return color; }
    const glm::vec3& getAmbientColor() const { return ambientColor; }
    const glm::vec3& getDiffuseColor() const { return diffuseColor; }
    const glm::vec3& getSpecularColor() const { return specularColor; }
    const glm::mat4& getLightSpaceMatrix() const { return lightSpaceMatrix; }
    const float getConstant() const { return constant; }
    const float getLinear() const { return linear; }
    const float getQuadratic() const { return quadratic; }
    friend class GUI;
private:
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 ambientColor;
    glm::vec3 diffuse;
    glm::vec3 diffuseColor;
    glm::vec3 specular;
    glm::vec3 specularColor;
    const float constant = 1.0f;
    const float linear = 0.07f;
    const float quadratic = 0.01f;

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 100.0f;
};

#endif // LIGHT_H

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Skybox.h"
#include <learnopengl/light.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <glm/glm.hpp>
#include <learnopengl/filesystem.h>
#include "Object.h"

class Scene {
public:
    Scene() = default;

    // Add objects and lights to the scene
    void addObject(std::unique_ptr<Object>&& object);
    void addLight(const Light& light);

    void setSkybox(const Skybox& skybox);
    void setFloor(const Object& floor);
    const Skybox& getSkybox() const;

    // Object management
    const std::vector<std::unique_ptr<Object>>& getObjects() const;
    const std::vector<Light>& getLights() const;

    std::vector<std::unique_ptr<Object>> objects;
    std::vector<Light> lights;
    Object floor;
    Skybox skybox;
};

#endif // SCENE_H
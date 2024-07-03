#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Skybox.h"
#include <utils/light.h>
#include <utils/shader.h>
#include <utils/camera.h>
#include <glm/glm.hpp>
#include <utils/filesystem.h>
#include "Object.h"
#include "PBRObject.h"

class Scene {
public:
    Scene() = default;

    void draw(Shader& shader, bool drawFloor = true);

    // Add objects and lights to the scene
    void addObject(std::unique_ptr<Object>&& object);
    void addLight(const Light& light);

    void setSkybox(const Skybox& skybox);
    void setFloor(const Object& floor);

    // Object management
    Skybox& getSkybox();
    std::vector<std::unique_ptr<Object>>& getObjects();
    std::vector<Light>& getLights();

    std::vector<std::unique_ptr<Object>> objects;
    std::vector<Light> lights;
    Object floor;
    Skybox skybox;
};

#endif // SCENE_H
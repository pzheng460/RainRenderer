#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <utils/filesystem.h>

#include <vector>

#include "Skybox.h"
#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include "Object.h"

class Scene {
public:
    Scene() = default;

    // Add objects and lights to the scene
    void addObject(const Object& object);
    void addLight(const Light& light);

    void setSkybox(const Skybox& skybox);
    void setFloor(const Object& floor);

    // Object management
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;
    std::unique_ptr<Object> floor;
    std::unique_ptr<Skybox> skybox;
    std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 10.0f, 30.0f));
};

#endif // SCENE_H
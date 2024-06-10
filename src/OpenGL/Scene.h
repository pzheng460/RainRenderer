#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Object.h"
#include "Skybox.h"
#include <learnopengl/light.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <glm/glm.hpp>
#include <learnopengl/filesystem.h>

class Scene {
public:
    Scene();
    ~Scene();

    // Add objects and lights to the scene
    void addObject(std::unique_ptr<Object>&& object);
    void addLight(const Light& light);

    // Skybox management
    void setSkybox(const Skybox& skybox);
    const Skybox& getSkybox() const;

    // Object management
    const std::vector<std::unique_ptr<Object>>& getObjects() const;
    const std::vector<Light>& getLights() const;

    // Camera management
    void setCamera(const Camera& camera);
    const Camera& getCamera() const;

    friend class Renderer;
    friend class PBRRenderer;
    friend int main();

private:
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<Light> lights;
    Skybox skybox = Skybox(Shader(FileSystem::getPath("src/OpenGL/shaders/background.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/background.fs").c_str()), FileSystem::getPath("resources/textures/hdr/newport_loft.hdr"));
    Camera camera = glm::vec3(0.0f, 10.0f, 30.0f);
};

#endif // SCENE_H
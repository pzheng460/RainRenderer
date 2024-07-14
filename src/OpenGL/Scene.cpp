#include "Scene.h"

void Scene::addObject(const Object& object) {
    objects.push_back(std::make_unique<Object>(object));
}

void Scene::addLight(const Light& light) {
    lights.push_back(std::make_unique<Light>(light));
}

void Scene::setFloor(const Object& floor) {
    this->floor = std::make_unique<Object>(floor);
}

void Scene::setSkybox(const Skybox& skybox) {
    this->skybox = std::make_unique<Skybox>(skybox);
}

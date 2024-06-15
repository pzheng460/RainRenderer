#include "Scene.h"

void Scene::addObject(std::unique_ptr<Object>&& object) {
    objects.push_back(std::move(object));
}

void Scene::addLight(const Light& light) {
    lights.push_back(light);
}

void Scene::setFloor(const Object& floor) {
    this->floor = floor;
}

void Scene::setSkybox(const Skybox& skybox) {
    this->skybox = skybox;
}

const Skybox& Scene::getSkybox() const {
    return skybox;
}

const std::vector<std::unique_ptr<Object>>& Scene::getObjects() const {
    return objects;
}

const std::vector<Light>& Scene::getLights() const {
    return lights;
}

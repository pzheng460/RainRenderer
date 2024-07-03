#include "Scene.h"
#include "shaderSetting.h"

void Scene::draw(Shader& shader, bool drawFloor) {
    // render floor 渲染地板
    if (drawFloor) {
        floor.draw(shader);
    }

    // render objects 渲染物体
    for (auto& obj : objects) {
        obj->draw(shader);
    }
}

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

Skybox& Scene::getSkybox() {
    return skybox;
}

std::vector<std::unique_ptr<Object>>& Scene::getObjects() {
    return objects;
}

std::vector<Light>& Scene::getLights() {
    return lights;
}

#include "Scene.h"
#include "shaderSetting.h"

void Scene::drawScene(Shader& shader, bool drawFloor, bool drawPBRObject) {
    // render floor 渲染地板
    if (drawFloor) {
        floor.setShader(shader);
        floor.draw();
    }

    // render objects 渲染物体
    for (auto& obj : objects) {
        if (auto pbrObj = dynamic_cast<PBRObject*>(obj.get())) {
            if (drawPBRObject) {
                PBRShaderSetting(obj->getShader(), lights, skybox.getIrradianceMap(), skybox.getPrefilterMap(), skybox.getBRDFLUTTexture());
                pbrObj->draw();
            }
        } else {
            obj->setShader(shader);
            obj->draw();
        }
    }
}

void Scene::addObject(std::unique_ptr<Object>&& object) {
    objects.push_back(std::move(object));
}

void Scene::addLight(const Light& light) {
    lights.push_back(light);
    ShadowMap shadowMap;
    shadowMaps.push_back(shadowMap);
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

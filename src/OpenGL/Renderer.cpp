#include "Renderer.h"
#include <stb_image.h>
#include <iostream>
#include "render_implicit_geometry.h"

Renderer::Renderer(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, Scene& scene)
        : SCR_WIDTH(SCR_WIDTH), SCR_HEIGHT(SCR_HEIGHT), scene(scene),
          shader(FileSystem::getPath("src/OpenGL/shaders/model_loading.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/model_loading.fs").c_str()) {}

void Renderer::initialize() {}

void Renderer::renderScene(std::shared_ptr<GUI> gui) {
    if (gui->IsSkyBoxActive()) {
        scene.skybox.drawSkybox(scene.getCamera(), SCR_WIDTH, SCR_HEIGHT);
    }

    for (auto& obj : scene.getObjects()) {
        if (obj->getRenderMode() == NORMAL) {
            shader.use();
            shader.setMat4("model", obj->getModelMatrix());
            shader.setMat4("view", obj->getViewMatrix());
            shader.setMat4("projection", obj->getProjectionMatrix());

            // 使用 std::holds_alternative 和 std::get 进行类型检查和处理
            if (std::holds_alternative<Model>(obj->data)) {
                std::get<Model>(obj->data).Draw(shader);
            } else if (std::holds_alternative<ImplicitGeometryType>(obj->data)) {
                renderGeometry(std::get<ImplicitGeometryType>(obj->data));
            }
        }
    }
}

void Renderer::renderGeometry(ImplicitGeometryType geometryType) {
    switch (geometryType) {
        case SPHERE:
            renderSphere();
            break;
        case CUBE:
            renderCube();
            break;
        case QUAD:
            renderQuad();
            break;
        default:
            std::cerr << "Unknown geometry type" << std::endl;
            break;
    }
}




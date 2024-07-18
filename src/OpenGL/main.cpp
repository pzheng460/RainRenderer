#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <utils/filesystem.h>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"

#include <iostream>

#include "Object.h"
#include "GUI.h"
#include "MainWindow.h"
#include "Scene.h"
#include "Renderer.h"
#include "Common.h"

int main()
{
    // Initialize GUI with all the settings 初始化GUI（GUI包含所有的参数设定）
    auto gui = std::make_shared<GUI>();

    // initialize scene 初始化scene类
    Scene scene;

    MainWindow::camera = scene.camera;
    MainWindow::gui = gui;

    // bind GUI to Window 绑定GUI到Window
    MainWindow mainWindow(SCR_WIDTH, SCR_HEIGHT, "Rein Renderer GL");

    if (!mainWindow.init()) {
        return -1;
    }

    // load skybox 加载天空盒
    if (gui->skyboxMode == SkyboxLoadMode::CUBE_MAP)
    {
        std::vector<std::string> paths =
                {
                        FileSystem::getPath("resources/textures/skybox/right.jpg"),
                        FileSystem::getPath("resources/textures/skybox/left.jpg"),
                        FileSystem::getPath("resources/textures/skybox/top.jpg"),
                        FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
                        FileSystem::getPath("resources/textures/skybox/front.jpg"),
                        FileSystem::getPath("resources/textures/skybox/back.jpg")
                };
        Skybox skybox(paths);
        scene.setSkybox(skybox);
    }
    else if (gui->skyboxMode == SkyboxLoadMode::SPHERE_MAP)
    {
        Skybox skybox(FileSystem::getPath("resources/textures/hdr/newport_loft.hdr"));
        scene.setSkybox(skybox);
    }

    // load floor 加载地板
    auto floorModel = Model(GeometryType::PLANE);
    floorModel.addTexture2D(FileSystem::getPath("resources/textures/wood.png"), DIFFUSE_NAME);
    auto floor = Object(floorModel);
    scene.setFloor(floor);

    // load PBR Sphere 加载PBR球体
    auto sphereModel = Model(GeometryType::SPHERE);
    sphereModel.addTexture2D(FileSystem::getPath("resources/textures/pbr/rusted_iron/albedo.png"), PBR_ALBEDO_NAME);
    sphereModel.addTexture2D(FileSystem::getPath("resources/textures/pbr/rusted_iron/normal.png"), PBR_NORMAL_NAME);
    sphereModel.addTexture2D(FileSystem::getPath("resources/textures/pbr/rusted_iron/metallic.png"), PBR_METALLIC_NAME);
    sphereModel.addTexture2D(FileSystem::getPath("resources/textures/pbr/rusted_iron/roughness.png"), PBR_ROUGHNESS_NAME);
    sphereModel.addTexture2D(FileSystem::getPath("resources/textures/pbr/rusted_iron/ao.png"), PBR_AO_NAME);
    auto pbrSphere = Object(sphereModel);
    scene.addObject(pbrSphere);

    // load lights 加载光源
    Light light0(glm::vec3(-10.0f, 10.0f, 10.0f), glm::vec3(1.0f));
    Light light1(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f));
    Light light2(glm::vec3(-10.0f, -10.0f, 10.0f), glm::vec3(1.0f));
    Light light3(glm::vec3(10.0f, -10.0f, -10.0f), glm::vec3(1.0f));


    scene.addLight(light0);
//    scene.addLight(light1);
//    scene.addLight(light2);
//    scene.addLight(light3);

    auto realScreen = mainWindow.reset();
    Renderer renderer(realScreen.first, realScreen.second, *gui, scene);

    // Initialize Dear ImGui 初始化Dear ImGui
    gui->init(mainWindow.getGLFWwindow());

    // render loop 渲染循环
    while (!mainWindow.shouldClose())
    {
        // reprocess before rendering 在渲染之前的预处理
        mainWindow.preRender();

        auto newScreen = mainWindow.reset();
        if (newScreen != realScreen) {
            realScreen = newScreen;
            renderer.setAllSize(realScreen.first, realScreen.second);
            renderer.init();
        }

        // render everything 渲染所有
        renderer.draw();

        // render Dear ImGui 渲染 Dear ImGui
        gui->render(scene);

        // swap buffers and poll IO events (keys pressed/released, mouse moved etc.) 交换缓冲区并轮询IO事件（按键按下/释放、鼠标移动等）
        mainWindow.swapBuffersAndPollEvents();
    }

    return 0;
}
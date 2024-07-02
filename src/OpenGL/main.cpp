#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <utils/filesystem.h>
#include <utils/shader.h>
#include <utils/camera.h>
#include <utils/model.h>
#include <utils/light.h>

#include <iostream>

#include "Object.h"
#include "PBRObject.h"
#include "GUI.h"
#include "MainWindow.h"
#include "Scene.h"
#include "shaderSetting.h"
#include "render_implicit_geometry.h"
#include "Renderer.h"

// settings 窗口设置
constexpr unsigned int SCR_WIDTH = 1280;
constexpr unsigned int SCR_HEIGHT = 720;

std::string modelFilePath = "resources/objects/YYB Symphony Miku by HB-Squiddy/yyb Symphony Miku by HB-Squiddy.pmx";
bool MainWindow::firstMouse = true;
float MainWindow::lastX = (float) SCR_WIDTH / 2.0;
float MainWindow::lastY = (float) SCR_HEIGHT / 2.0;

int main()
{
    // Initialize GUI with all the settings 初始化GUI（GUI包含所有的参数设定）
    auto gui = std::make_shared<GUI>();

    // bind GUI to Window 绑定GUI到Window
    MainWindow::gui = gui;
    MainWindow mainWindow(SCR_WIDTH, SCR_HEIGHT, "Rein Renderer GL");

    if (!mainWindow.init()) {
        return -1;
    }

    auto realScreen = mainWindow.reset();
    Renderer renderer(realScreen.first, realScreen.second, gui);

    // initialize scene 初始化scene类
    Scene scene;

    // load skybox 加载天空盒
    if (gui->getSkyboxLoadMode() == CUBEMAP) {
        Shader skyboxShader(FileSystem::getPath("src/OpenGL/shaders/skybox.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/skybox.fs").c_str());
        vector<std::string> faces =
                {
                        FileSystem::getPath("resources/textures/skybox/right.jpg"),
                        FileSystem::getPath("resources/textures/skybox/left.jpg"),
                        FileSystem::getPath("resources/textures/skybox/top.jpg"),
                        FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
                        FileSystem::getPath("resources/textures/skybox/front.jpg"),
                        FileSystem::getPath("resources/textures/skybox/back.jpg")
                };
        Skybox skybox(skyboxShader, faces);
        scene.setSkybox(skybox);
    } else if (gui->getSkyboxLoadMode() == SPHEREMAP) {
        Shader skyboxShader(FileSystem::getPath("src/OpenGL/shaders/background.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/background.fs").c_str());
        Skybox skybox(skyboxShader, FileSystem::getPath("resources/textures/hdr/newport_loft.hdr"));
        scene.setSkybox(skybox);
    }

    // load floor 加载地板
    Object floor(PLANE, true, "resources/textures/wood.png");
    scene.setFloor(floor);

    // load PBR Sphere 加载PBR球体
    auto object = std::make_unique<PBRObject>(SPHERE);
    scene.addObject(std::move(object));

    // load lights 加载光源
    Light light0(glm::vec3(-10.0f,  10.0f, 10.0f), glm::vec3(1.0f));
    Light light1(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f));
    Light light2(glm::vec3(-10.0f, -10.0f, 10.0f), glm::vec3(1.0f));
    Light light3(glm::vec3(10.0f, -10.0f, -10.0f), glm::vec3(1.0f));

    scene.addLight(light0);
//    scene.addLight(light1);
//    scene.addLight(light2);
//    scene.addLight(light3);

    renderer.init(scene);

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
            renderer.setFrameBufferSize(realScreen);
            renderer.init(scene);
        }

        // update MVP 更新MVP
        scene.skybox.setMVP(gui->getCamera(), SCR_WIDTH, SCR_HEIGHT);
        scene.floor.setMVP(gui->getCamera(), SCR_WIDTH, SCR_HEIGHT);

        for (auto& light : scene.lights) {
            light.setMVP(gui->getCamera(), SCR_WIDTH, SCR_HEIGHT);
        }

        for (auto& obj : scene.objects) {
            obj->setMVP(gui->getCamera(), SCR_WIDTH, SCR_HEIGHT);
        }

        // render everything 渲染所有
        renderer.draw(scene);

        // render Dear ImGui 渲染 Dear ImGui
        gui->render(modelFilePath, scene);

        // swap buffers and poll IO events (keys pressed/released, mouse moved etc.) 交换缓冲区并轮询IO事件（按键按下/释放、鼠标移动等）
        mainWindow.swapBuffersAndPollEvents();
    }

    return 0;
}
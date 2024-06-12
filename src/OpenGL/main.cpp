#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <learnopengl/light.h>

#include <iostream>

#include "Object.h"
#include "PBRObject.h"
#include "GUI.h"
#include "Window.h"
#include "Scene.h"

// settings 窗口设置
constexpr unsigned int SCR_WIDTH = 1280;
constexpr unsigned int SCR_HEIGHT = 720;

std::string modelFilePath = "resources/objects/YYB Symphony Miku by HB-Squiddy/yyb Symphony Miku by HB-Squiddy.pmx";
bool Window::firstMouse = true;
Camera Window::camera(glm::vec3(0.0f, 10.0f, 30.0f));
float Window::lastX = 800.0f / 2.0;
float Window::lastY = 600.0f / 2.0;

int main()
{
    // Initialize GUI with all the settings 初始化GUI（GUI包含所有的参数设定）
    auto gui = std::make_shared<GUI>();

    // bind GUI to Window 绑定GUI到Window
    Window::gui = gui;
    Window window(SCR_WIDTH, SCR_HEIGHT, "Rein Renderer GL");

    if (!window.init()) {
        return -1;
    }

    // initialize scene 初始化scene类
    Scene scene;

    // PBR Sphere PBR球体
    Shader pbrShader(FileSystem::getPath("src/OpenGL/shaders/pbr.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/pbr.fs").c_str());
    auto object = std::make_unique<PBRObject>(SPHERE, pbrShader);
    object->setMVP(window.getCamera(), SCR_WIDTH, SCR_HEIGHT);
    scene.addObject(std::move(object));

    // lights 光源
    Light light0(glm::vec3(-10.0f,  10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f));
    Light light1(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f));
    Light light2(glm::vec3(-10.0f, -10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f));
    Light light3(glm::vec3(10.0f, -10.0f, -10.0f), glm::vec3(300.0f, 300.0f, 300.0f));

    scene.addLight(light0);
    scene.addLight(light1);
    scene.addLight(light2);
    scene.addLight(light3);

    // Initialize Dear ImGui 初始化Dear ImGui
    gui->init(window.getGLFWwindow());

    // render loop 渲染循环
    while (!window.shouldClose())
    {
        // reprocess before rendering 在渲染之前的预处理
        window.preRender();

        // update MVP 更新MVP
        scene.skybox.setMVP(window.getCamera(), SCR_WIDTH, SCR_HEIGHT);

        for (auto& light : scene.lights) {
            light.setMVP(window.getCamera(), SCR_WIDTH, SCR_HEIGHT);
        }

        for (auto& obj : scene.objects) {
            obj->setMVP(window.getCamera(), SCR_WIDTH, SCR_HEIGHT);
        }

        // render scene 渲染场景
        // render skybox 渲染天空盒
        if (gui->IsSkyBoxActive()) {
            scene.skybox.drawSkybox();
        }

        // render lights 渲染光源
        if (gui->IsLightActive()) {
            for (int i = 0; i < scene.lights.size(); ++i) {
                scene.lights[i].draw();
            }
        }

        // render objects 渲染物体
        for (auto& obj : scene.getObjects()) {
            if (auto pbrObj = dynamic_cast<PBRObject*>(obj.get())) {
                if (gui->IsPBRActive()) {
                    pbrObj->draw(scene.lights, scene.skybox.getIrradianceMap(), scene.skybox.getPrefilterMap(), scene.skybox.getBRDFLUTTexture());
                }
            } else {
                obj->draw();
            }
        }

        // render Dear ImGui 渲染 Dear ImGui
        gui->render(window.getCamera(), modelFilePath, scene);

        // swap buffers and poll IO events (keys pressed/released, mouse moved etc.) 交换缓冲区并轮询IO事件（按键按下/释放、鼠标移动等）
        window.swapBuffersAndPollEvents();
    }

    return 0;
}
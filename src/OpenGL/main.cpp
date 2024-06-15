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
float Window::lastX = (float) SCR_WIDTH / 2.0;
float Window::lastY = (float) SCR_HEIGHT / 2.0;

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
    Shader floorShader(FileSystem::getPath("src/OpenGL/shaders/model_loading.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/model_loading.fs").c_str());
    Object floor(PLANE, floorShader, true, "resources/textures/metal.png");
    scene.setFloor(floor);

    // load PBR Sphere 加载PBR球体
    Shader pbrShader(FileSystem::getPath("src/OpenGL/shaders/pbr.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/pbr.fs").c_str());
    auto object = std::make_unique<PBRObject>(SPHERE, pbrShader);
    object->setMVP(window.getCamera(), SCR_WIDTH, SCR_HEIGHT);
    scene.addObject(std::move(object));

    // load lights 加载光源
    Light light0(glm::vec3(-10.0f,  10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Light light1(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Light light2(glm::vec3(-10.0f, -10.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Light light3(glm::vec3(10.0f, -10.0f, -10.0f), glm::vec3(1.0f, 1.0f, 1.0f));

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

        // face culling 面剔除
        window.faceCulling(gui->IsFaceCullingActive());

        // update MVP 更新MVP
        scene.skybox.setMVP(window.getCamera(), SCR_WIDTH, SCR_HEIGHT);
        scene.floor.setMVP(window.getCamera(), SCR_WIDTH, SCR_HEIGHT);

        for (auto& light : scene.lights) {
            light.setMVP(window.getCamera(), SCR_WIDTH, SCR_HEIGHT);
        }

        for (auto& obj : scene.objects) {
            obj->setMVP(window.getCamera(), SCR_WIDTH, SCR_HEIGHT);
        }

        // render lights 渲染光源
        if (gui->IsLightActive()) {
            for (int i = 0; i < scene.lights.size(); ++i) {
                scene.lights[i].draw();
            }
        }

        // render floor 渲染地板
        if (gui->IsFloorActive()) {
            scene.floor.basicShaderSetting();
            scene.floor.draw();
        }

        // render objects 渲染物体
        for (auto& obj : scene.objects) {
            if (auto pbrObj = dynamic_cast<PBRObject*>(obj.get())) {
                if (gui->IsPBRActive()) {
                    pbrObj->PBRShaderSetting(scene.lights, scene.skybox.getIrradianceMap(), scene.skybox.getPrefilterMap(), scene.skybox.getBRDFLUTTexture());
                    pbrObj->draw();
                }
            } else {
                if (gui->IsOutlineActive()) {
                    window.preOutlineSetting();
                }

                if (gui->getMode() == BASIC) {
                    Shader basicShader(FileSystem::getPath("src/OpenGL/shaders/model_loading.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/model_loading.fs").c_str());
                    obj->setShader(basicShader);
                    obj->basicShaderSetting();
                } else if (gui->getMode() == PHONG) {
                    Shader phongShader(FileSystem::getPath("src/OpenGL/shaders/phong.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/phong.fs").c_str());
                    obj->setShader(phongShader);
                    obj->phongShaderSetting(window.getCamera(), scene.lights);
                } else if (gui->getMode() == DEPTH) {
                    Shader depthShader(FileSystem::getPath("src/OpenGL/shaders/depth_testing.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/depth_testing.fs").c_str());
                    obj->setShader(depthShader);
                    obj->basicShaderSetting();
                } else if (gui->getMode() == ENVIRONMENTMAPPING) {
                    Shader environmentMappingShader(FileSystem::getPath("src/OpenGL/shaders/environment_mapping.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/environment_mapping.fs").c_str());
                    obj->setShader(environmentMappingShader);
                    obj->phongShaderSetting(window.getCamera(), scene.lights);
                }
                obj->draw();

                if (gui->IsOutlineActive()) {
                    window.outlineSetting();
                    Shader outlineShader(FileSystem::getPath("src/OpenGL/shaders/model_loading.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/stencil_single_color.fs").c_str());
                    obj->setShader(outlineShader);
                    obj->setScale(1.01f);
                    obj->setMVP(window.getCamera(), SCR_WIDTH, SCR_HEIGHT);
                    obj->basicShaderSetting();
                    obj->draw();
                    obj->setScale(1.0f);
                    window.afterOutlineSetting();
                }
            }
        }

        // render skybox 渲染天空盒，放在最后渲染保证early-z测试
        if (gui->IsSkyBoxActive()) {
            if (gui->getSkyboxLoadMode() == CUBEMAP) {
                scene.skybox.drawGeometry();
            } else if (gui->getSkyboxLoadMode() == SPHEREMAP) {
                scene.skybox.draw();
            }
        }

        // render Dear ImGui 渲染 Dear ImGui
        gui->render(window.getCamera(), modelFilePath, scene);

        // swap buffers and poll IO events (keys pressed/released, mouse moved etc.) 交换缓冲区并轮询IO事件（按键按下/释放、鼠标移动等）
        window.swapBuffersAndPollEvents();
    }

    return 0;
}
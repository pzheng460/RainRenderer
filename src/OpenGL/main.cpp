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
#include <learnopengl/control.h>
#include <learnopengl/light.h>

#include <iostream>

#include "Object.h"
#include "PBRObject.h"
#include "GUI.h"
#include "Window.h"
#include "Scene.h"
#include "Renderer.h"
#include "PBRRenderer.h"

// settings
constexpr unsigned int SCR_WIDTH = 1280;
constexpr unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 30.0f));
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

// control
bool mouseControlEnabled = false;

std::string modelFilePath = "resources/objects/YYB Symphony Miku by HB-Squiddy/yyb Symphony Miku by HB-Squiddy.pmx";

int main()
{
    Window window(SCR_WIDTH, SCR_HEIGHT, "Rein Renderer GL");

    if (!window.init()) {
        return -1;
    }

    Scene scene;

    // PBR Sphere
    auto object = std::make_unique<PBRObject>(SPHERE);
    object->setMVP(camera, SCR_WIDTH, SCR_HEIGHT);
    scene.addObject(std::move(object));

    // lights
    // ------
    Light light0(glm::vec3(-10.0f,  10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f));
    Light light1(glm::vec3(10.0f, 10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f));
    Light light2(glm::vec3(-10.0f, -10.0f, 10.0f), glm::vec3(300.0f, 300.0f, 300.0f));
    Light light3(glm::vec3(10.0f, -10.0f, -10.0f), glm::vec3(300.0f, 300.0f, 300.0f));

    scene.addLight(light0);
    scene.addLight(light1);
    scene.addLight(light2);
    scene.addLight(light3);

    // Dear ImGui
    GUI gui;
    gui.init(window.getGLFWwindow());

    // render loop
    // -----------
    while (!window.shouldClose())
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window.getGLFWwindow());

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render scene, supplying the convoluted irradiance map to the final shader.
        // ------------------------------------------------------------------------------------------
        for (auto& obj : scene.objects) {
            obj->setMVP(camera, SCR_WIDTH, SCR_HEIGHT);
        }

        Renderer renderer(SCR_WIDTH, SCR_HEIGHT, scene);
        renderer.initialize();
        renderer.renderScene(gui);

        if (gui.IsPBRActive()) {
            PBRRenderer pbrRenderer(SCR_WIDTH, SCR_HEIGHT, scene);
            pbrRenderer.initialize();
            pbrRenderer.renderScene(gui);
        }

        // Dear ImGui render
        gui.render(camera, scene.objects, modelFilePath, scene.lights);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        window.swapBuffersAndPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    return 0;
}
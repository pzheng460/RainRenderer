#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <learnopengl/camera.h>
#include "GUI.h"

class Window {
public:
    Window(unsigned int width, unsigned int height, const std::string& title);
    ~Window();

    bool init();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    bool shouldClose() const;
    void preRender();
    void reset();
    void processInput(GLFWwindow *window);
    void swapBuffersAndPollEvents() const;

    void preOutlineSetting() const;
    void outlineSetting() const;
    void afterOutlineSetting() const;

    void faceCulling(bool faceCullingActive) const;
    void MSAA(bool MSAAActive) const;
    void gammaCorrection(bool gammaActive) const;

    // GUI
    static std::shared_ptr<GUI> gui;
    // control
    static bool firstMouse;
    // camera
    static Camera camera;
    static float lastX;
    static float lastY;

    GLFWwindow* getGLFWwindow() const {
        return window;
    }

    Camera& getCamera() const {
        return camera;
    }

private:
    unsigned int width;
    unsigned int height;
    std::string title;
    GLFWwindow* window;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

#endif // WINDOW_H

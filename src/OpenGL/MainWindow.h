#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.h"
#include "GUI.h"

class MainWindow {
public:
    MainWindow(unsigned int width, unsigned int height, const std::string& title);
    ~MainWindow();

    bool init();

    bool shouldClose() const;
    void preRender();
    std::pair<int, int> reset();
    void processInput(GLFWwindow *window);
    void swapBuffersAndPollEvents() const;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    // GUI
    static std::shared_ptr<GUI> gui;
    // control
    static bool firstMouse;
    // camera
    static float lastX;
    static float lastY;

    GLFWwindow* getGLFWwindow() const {
        return window;
    }

private:
    // window settings
    int width;
    int height;
    std::string title;
    GLFWwindow* window;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

#endif // WINDOW_H

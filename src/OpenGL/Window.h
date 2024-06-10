#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
    Window(unsigned int width, unsigned int height, const std::string& title);
    ~Window();

    bool init();
    bool shouldClose() const;
    void swapBuffersAndPollEvents() const;

    GLFWwindow* getGLFWwindow() const;

    static void callback(GLFWwindow* window);

private:
    unsigned int width;
    unsigned int height;
    std::string title;
    GLFWwindow* window;
};

#endif // WINDOW_H

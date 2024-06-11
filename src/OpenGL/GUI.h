#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <learnopengl/camera.h>
#include <learnopengl/light.h>

#include "Object.h"

class GUI {
public:
    GUI();
    ~GUI();
    void init(GLFWwindow* window);
    void render(Camera& camera, std::vector<unique_ptr<Object>>& objects, std::string& modelFilePath, std::vector<Light>& lights);
    bool IsPBRActive() const {
        return pbrActive;
    }
    bool IsSkyBoxActive() const {
        return skyBoxActive;
    }
    bool IsControlActive() const {
        return controlActive;
    }

    void ToggleControl() {
        controlActive = !controlActive;
    }

private:
    bool pbrActive = true;
    bool skyBoxActive = true;
    bool controlActive = false;
};

#endif // GUI_H

#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Object.h"

#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <learnopengl/camera.h>
#include <learnopengl/light.h>

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
    bool IsWireframeMode() const {
        return wireframeMode;
    }
    // 其他功能开关和参数的 getter
private:
    bool pbrActive = true;
    bool skyBoxActive = true;

    bool wireframeMode = false;
    // 其他功能开关和参数
};

#endif // GUI_H

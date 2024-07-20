#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Scene.h"
#include "Common.h"

class GUI {
public:
    GUI() = default;
    ~GUI() = default;
    void init(GLFWwindow* window);
    void render(Scene& scene);

    void ToggleControl() {
        controlActive = !controlActive;
    }

    RenderMode mode = RenderMode::BASIC;
    SkyboxLoadMode skyboxMode = SkyboxLoadMode::SPHERE_MAP;
    RenderingPath renderingPath = RenderingPath::FORWARD_RENDERING;

    bool controlActive = false;

    bool skyBoxActive = true;
    bool lightActive = true;
    bool floorActive = false;

    bool normalVisualizationActive = false;
    bool faceCullingActive = false;
    bool MSAAActive = true;
    bool gammaCorrectionActive = false;
    bool HDRActive = false;
    bool bloomActive = false;
    bool shadowActive = false;
    bool SSAOActive = false;
};

#endif // GUI_H

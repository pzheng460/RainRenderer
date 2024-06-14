#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <learnopengl/camera.h>
#include <learnopengl/light.h>
#include "Object.h"
#include "Scene.h"

enum renderMode {
    BASIC,
    PHONG,
    DEPTH
};

class GUI {
public:
    GUI();
    ~GUI();
    void init(GLFWwindow* window);
    void render(Camera& camera, std::string& modelFilePath, Scene& scene);

    renderMode getMode() const {
        return mode;
    }

    bool IsSkyBoxActive() const {
        return skyBoxActive;
    }
    bool IsLightActive() const {
        return lightActive;
    }
    bool IsFloorActive() const {
        return floorActive;
    }

    bool IsPBRActive() const {
        return pbrActive;
    }

    bool IsControlActive() const {
        return controlActive;
    }
    bool IsOutlineActive() const {
        return outlineActive;
    }
    bool IsFaceCullingActive() const {
        return faceCullingActive;
    }

    void ToggleControl() {
        controlActive = !controlActive;
    }

private:
    renderMode mode = BASIC;

    bool pbrActive = false;
    bool skyBoxActive = false;
    bool controlActive = false;
    bool lightActive = true;
    bool outlineActive = false;
    bool faceCullingActive = false;
    bool floorActive = true;
};

#endif // GUI_H

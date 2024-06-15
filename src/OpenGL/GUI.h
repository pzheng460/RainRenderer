#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <learnopengl/camera.h>
#include <learnopengl/light.h>
//#include "Object.h"
#include "Scene.h"

enum renderMode {
    BASIC,
    PHONG,
    DEPTH,
    ENVIRONMENTMAPPING
};

enum skyboxLoadMode {
    CUBEMAP,
    SPHEREMAP
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

    skyboxLoadMode getSkyboxLoadMode() const {
        return skyboxMode;
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
    skyboxLoadMode skyboxMode = SPHEREMAP;

    bool controlActive = false;

    bool pbrActive = false;

    bool skyBoxActive = true;
    bool lightActive = true;
    bool floorActive = false;

    bool outlineActive = false;
    bool faceCullingActive = false;
};

#endif // GUI_H

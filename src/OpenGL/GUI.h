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
    BLINNPHONG,
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

    bool IsControlActive() const {
        return controlActive;
    }

    bool IsPBRActive() const {
        return pbrActive;
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

    bool IsOutlineActive() const {
        return outlineActive;
    }
    bool IsNormalVisualizationActive() const {
        return normalVisualizationActive;
    }
    bool IsFaceCullingActive() const {
        return faceCullingActive;
    }
    bool IsMSAAActive() const {
        return MSAAActive;
    }
    bool IsGammaCorrectionActive() const {
        return gammaCorrectionActive;
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
    bool normalVisualizationActive = false;
    bool faceCullingActive = false;
    bool MSAAActive = true;
    bool gammaCorrectionActive = false;
};

#endif // GUI_H

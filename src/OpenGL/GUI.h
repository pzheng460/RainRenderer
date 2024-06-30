#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <learnopengl/camera.h>
#include <learnopengl/light.h>

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
    void render(std::string& modelFilePath, Scene& scene);

    Camera& getCamera() {
        return camera;
    }

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
    bool IsShadowActive() const {
        return shadowActive;
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
    bool IsHDRActive() const {
        return HDRActive;
    }
    bool IsBloomActive() const {
        return bloomActive;
    }
    bool IsSSAOActive() const {
        return SSAOActive;
    }

    void ToggleControl() {
        controlActive = !controlActive;
    }

private:
    renderMode mode = BASIC;
    skyboxLoadMode skyboxMode = SPHEREMAP;

    Camera camera = Camera(glm::vec3(0.0f, 10.0f, 30.0f));

    bool controlActive = false;

    bool pbrActive = false;

    bool skyBoxActive = false;
    bool lightActive = true;
    bool floorActive = true;
    bool shadowActive = false;

    bool outlineActive = false;
    bool normalVisualizationActive = false;
    bool faceCullingActive = false;
    bool MSAAActive = true;
    bool gammaCorrectionActive = false;
    bool HDRActive = false;
    bool bloomActive = false;
    bool SSAOActive = false;
};

#endif // GUI_H

#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "Scene.h"

enum RenderingPath {
    FORWARDRENDERING,
    DEFERREDRENDERING
};

enum RenderMode {
    BASIC,
    PHONG,
    BLINNPHONG,
    DEPTH,
    ENVIRONMENTMAPPING,
    PBR
};

enum SkyboxLoadMode {
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

    RenderingPath getRenderingPath() const {
        return renderingPath;
    }

    RenderMode getMode() const {
        return mode;
    }

    SkyboxLoadMode getSkyboxLoadMode() const {
        return skyboxMode;
    }

    bool IsControlActive() const {
        return controlActive;
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
    RenderMode mode = BASIC;
    SkyboxLoadMode skyboxMode = SPHEREMAP;
    RenderingPath renderingPath = FORWARDRENDERING;

    Camera camera = Camera(glm::vec3(0.0f, 10.0f, 30.0f));

    bool controlActive = false;

    bool skyBoxActive = false;
    bool lightActive = true;
    bool floorActive = true;

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

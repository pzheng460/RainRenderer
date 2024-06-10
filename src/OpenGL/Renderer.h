#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include "Object.h"
#include "GUI.h"
#include "Scene.h"

class Renderer {
public:
    Renderer(unsigned int screenWidth, unsigned int SCR_HEIGHT, Scene& scene);
    virtual ~Renderer() = default;

    virtual void initialize();
    virtual void renderScene(GUI& gui);

protected:
    Shader shader;
    Scene& scene;
    unsigned int SCR_WIDTH, SCR_HEIGHT;

    void renderGeometry(ImplicitGeometryType geometryType);
};

#endif // RENDERER_H
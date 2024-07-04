#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FrameBuffer.h"
#include "MSAAFrameBuffer.h"
#include "GeometryFrameBuffer.h"
#include <utils/filesystem.h>
#include <utils/shader.h>

#include <utility>
#include "Scene.h"
#include "GUI.h"
#include "SSAO.h"

#include <random>

class Renderer {
public:
    Renderer() = default;
    Renderer(int SCR_WIDTH, int SCR_HEIGHT, std::shared_ptr<GUI>& gui) : SCR_WIDTH(SCR_WIDTH), SCR_HEIGHT(SCR_HEIGHT), gui(gui) {}
    ~Renderer() = default;

    void init(Scene& scene);
    void reset();
    void draw(Scene& scene);
    void forwardRendering(Scene& scene);
    void deferredRendering(Scene& scene);

    void gaussianBlur();

    void faceCulling() const;
    void MSAA() const;
    void gammaCorrection() const;

    // GUI
    std::shared_ptr<GUI> gui;

    void setFrameBufferSize(pair<int, int> size) {
        SCR_WIDTH = size.first;
        SCR_HEIGHT = size.second;
    }

    FrameBuffer& getMainMSAAFrameBuffer() {
        return mainMSAAFrameBuffer;
    }

    FrameBuffer& getIntermediateFrameBuffer() {
        return intermediateFrameBuffer;
    }

    std::vector<FrameBuffer>& getPingPongFrameBuffers() {
        return pingPongFrameBuffers;
    }
private:
    // Main Shader
    Shader mainShader;

    int SCR_WIDTH, SCR_HEIGHT;

    bool firstIteration = true;

    // Shadow Map
    Shader simpleDepthShader = Shader(FileSystem::getPath("src/OpenGL/shaders/shadow_mapping_depth.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/shadow_mapping_depth.fs").c_str());
    std::vector<FrameBuffer> shadowMapFrameBuffers;
    const unsigned int SHADOW_WIDTH = 2560, SHADOW_HEIGHT = 1440;

    // Main FrameBuffer
    MSAAFrameBuffer mainMSAAFrameBuffer = MSAAFrameBuffer(2, 0, 1);
    FrameBuffer intermediateFrameBuffer = FrameBuffer(2, 0, 0);

    // Bloom
    Shader shaderBlur = Shader(FileSystem::getPath("src/OpenGL/shaders/blur.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/blur.fs").c_str());
    std::vector<FrameBuffer> pingPongFrameBuffers = std::vector<FrameBuffer>(2);
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;

    // G-Buffer
    Shader gFrameBufferShader = Shader(FileSystem::getPath("src/OpenGL/shaders/g_buffer.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/g_buffer.fs").c_str());
    GeometryFrameBuffer gFrameBuffer = GeometryFrameBuffer(3, 0, 1);

    // SSAO
    SSAO ssao = SSAO(gui->getCamera(), SCR_WIDTH, SCR_HEIGHT, gFrameBuffer);

    // Deferred lighting
    Shader deferredLightingShader = Shader(FileSystem::getPath("src/OpenGL/shaders/deferred_shading.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/deferred_shading.fs").c_str());

    // Final
    Shader finalShader = Shader(FileSystem::getPath("src/OpenGL/shaders/final.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/final.fs").c_str());
};

#endif // _RENDERER_H

#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <utils/filesystem.h>

#include <utility>

#include "FrameBuffer.h"
#include "Shader.h"
#include "Scene.h"
#include "GUI.h"
#include "SSAO.h"

#include <random>

class Renderer {
public:
    Renderer() = default;
    Renderer(int width, int height, std::shared_ptr<GUI>& gui, Scene& scene) :
            width(width), height(height), gui(gui) {
        for (int i = 0; i < scene.lights.size(); ++i) {
            shadowMapFrameBuffers.emplace_back(FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_SHADOW_MAP));
        }
        for (int i = 0; i < 2; ++i) {
            pingPongFrameBuffers.emplace_back(FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_PING_PONG));
        }
    }
    ~Renderer() = default;

    void init();
    void reset();
    void draw(Scene& scene);
    void forwardRendering(Scene& scene);
    void deferredRendering(Scene& scene);

    void gaussianBlur();

    void faceCulling() const;
    void MSAA() const;
    void gammaCorrection() const;

    void setSize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;
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

    // GUI
    std::shared_ptr<GUI> gui;

    int width, height;

    bool firstIteration = true;

    // Shadow Map
    Shader ShaderShadowMap = ShaderFactory::createShader(ShaderFactoryType::SHADER_SHADOW_MAP);
    Shader shaderShadowMapDebug = ShaderFactory::createShader(ShaderFactoryType::SHADER_SHADOW_MAP_DEBUG);
    std::vector<FrameBuffer> shadowMapFrameBuffers;
    const int SHADOW_WIDTH = 2560, SHADOW_HEIGHT = 1440;

    // Main FrameBuffer
    FrameBuffer mainMSAAFrameBuffer = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_MSAA);
    FrameBuffer intermediateFrameBuffer = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_INTERMEDIATE);

    // Normal Visualization
    Shader shaderNormalVisualization = ShaderFactory::createShader(ShaderFactoryType::SHADER_NORMAL_VISUALIZATION);

    // Bloom
    Shader shaderBloom = ShaderFactory::createShader(ShaderFactoryType::SHADER_BLOOM);
    std::vector<FrameBuffer> pingPongFrameBuffers;
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;

    // G-Buffer
    Shader gFrameBufferShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_GEOMETRY);
    FrameBuffer gFrameBuffer = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_GEOMETRY);

    // SSAO
    SSAO ssao = SSAO(gui->getCamera(), width, height, gFrameBuffer);

    // Deferred lighting
    Shader deferredLightingShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_DEFERRED_LIGHTING);

    // Final
    Shader finalShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_FINAL);
};

#endif // RENDERER_H

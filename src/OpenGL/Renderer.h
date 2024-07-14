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
#include "Renderer/SSAO/SSAO.h"

#include <random>

class Renderer {
public:
    Renderer(int width, int height, GUI& gui, Scene& scene);
    ~Renderer() = default;

    void init();
    void reset();
    void draw();

    void setSize(int newWidth, int newHeight);
private:
    void forwardRendering();
    void deferredRendering();

    void faceCulling() const;
    void MSAA() const;
    void gammaCorrection() const;

    // GUI
    int width, height;
    GUI& gui;
    Scene& scene;

    std::unique_ptr<Object> screenQuad;

    // Shared FrameBuffers
    std::shared_ptr<FrameBuffer> frameBufferDefault;
    std::shared_ptr<FrameBuffer> frameBufferMSAA;
    std::shared_ptr<FrameBuffer> frameBufferIntermediate;
    std::shared_ptr<FrameBuffer> frameBufferGeometry;
    std::shared_ptr<FrameBuffer> frameBufferSSAO;
    std::shared_ptr<FrameBuffer> frameBufferSSAOBlur;
    std::vector<std::shared_ptr<FrameBuffer>> frameBufferShadowMaps;
    std::vector<std::shared_ptr<FrameBuffer>> frameBufferBlooms;

    // Skybox
    std::shared_ptr<Shader> shaderSkyboxCubeMap;
    std::shared_ptr<Shader> shaderSkyboxSphereMap;
    void renderSkybox();

    std::shared_ptr<Shader> shaderSkyboxSphereMapToCubeMap;
    void renderSkyboxSphereMapToCubeMap();
    std::shared_ptr<Shader> shaderIrradiance;
    void renderIrradiance();
    std::shared_ptr<Shader> shaderPrefilter;
    void renderPrefilter();
    std::shared_ptr<Shader> shaderBRDFLUT;
    void renderBRDFLUT();

    // Light
    std::shared_ptr<Shader> shaderLight;
    void renderLight();

    // Shadow Map
    std::shared_ptr<Shader> shaderShadowMap;
    void renderShadowMap();
    std::shared_ptr<Shader> shaderShadowMapDebug;
    void renderShadowMapDebug();
    const int SHADOW_WIDTH = 2560, SHADOW_HEIGHT = 1440;

    // Basic
    std::shared_ptr<Shader> shaderBasic;
    void renderBasic();

    // Phong
    std::shared_ptr<Shader> shaderPhong;
    void renderPhong();

    // Blinn-Phong
    std::shared_ptr<Shader> shaderBlinnPhong;
    void renderBlinnPhong();

    // Depth Testing
    std::shared_ptr<Shader> shaderDepthTesting;
    void renderDepthTesting();

    // Environment Mapping
    std::shared_ptr<Shader> shaderEnvironmentMapping;
    void renderEnvironmentMapping();

    // PBR
    std::shared_ptr<Shader> shaderPBR;
    void renderPBR();

    // Normal Visualization
    std::shared_ptr<Shader> shaderNormalVisualization;
    void renderNormalVisualization();

    // Bloom
    std::shared_ptr<Shader> shaderBloom;
    void renderBloom();
    bool horizontal = true;
    unsigned int amount = 10;

    // G-Buffer
    std::shared_ptr<Shader> shaderGeometry;
    void renderGeometry();

    // SSAO
    std::shared_ptr<Shader> shaderSSAO;
    void renderSSAO();
    SSAO ssao = SSAO();

    std::shared_ptr<Shader> shaderSSAOBlur;
    void renderSSAOBlur();

    // Deferred lighting
    std::shared_ptr<Shader> shaderDeferredLighting;
    void renderDeferredLighting();

    // Post Processing
    std::shared_ptr<Shader> shaderPostProcessing;
    void renderPostPossing();
};

#endif // RENDERER_H

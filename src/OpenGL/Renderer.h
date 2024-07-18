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
    void draw();

    void setSize(int newWidth, int newHeight);
    void setAllSize(int newWidth, int newHeight);
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
    std::shared_ptr<FrameBuffer> frameBufferSkyboxCapture;
    std::vector<std::shared_ptr<FrameBuffer>> frameBufferShadowMaps;
    std::vector<std::shared_ptr<FrameBuffer>> frameBufferBlooms;

    // Skybox
    std::shared_ptr<Shader> shaderSkybox;
    std::shared_ptr<Shader> shaderSkyboxHDR;
    void renderSkybox(FrameBuffer* frameBuffer);
    void renderSkyboxHDR(FrameBuffer* frameBuffer);

    std::shared_ptr<Shader> shaderSkyboxSphereMapToCubeMap;
    void renderSkyboxSphereMapToCubeMap(FrameBuffer* frameBuffer);
    std::shared_ptr<Shader> shaderIrradiance;
    void renderIrradiance(FrameBuffer* frameBuffer);
    std::shared_ptr<Shader> shaderPrefilter;
    void renderPrefilter(FrameBuffer* frameBuffer);
    std::shared_ptr<Shader> shaderBRDFLUT;
    void renderBRDFLUT(FrameBuffer* frameBuffer);

    // Light
    std::shared_ptr<Shader> shaderLight;
    void renderLight(FrameBuffer* frameBuffer);

    // Shadow Map
    std::shared_ptr<Shader> shaderShadowMap;
    void renderShadowMap(FrameBuffer* frameBuffer, int i);
    std::shared_ptr<Shader> shaderShadowMapDebug;
    void renderShadowMapDebug(FrameBuffer* frameBuffer, int i);

    int shadowWidth = 4960, shadowHeight = 4960;

    // Basic
    std::shared_ptr<Shader> shaderBasic;
    void renderBasic(FrameBuffer* frameBuffer);

    // Phong
    std::shared_ptr<Shader> shaderPhong;
    void renderPhong(FrameBuffer* frameBuffer);

    // Blinn-Phong
    std::shared_ptr<Shader> shaderBlinnPhong;
    void renderBlinnPhong(FrameBuffer* frameBuffer);

    // Depth Testing
    std::shared_ptr<Shader> shaderDepthTesting;
    void renderDepthTesting(FrameBuffer* frameBuffer);

    // Environment Mapping
    std::shared_ptr<Shader> shaderEnvironmentMapping;
    void renderEnvironmentMapping(FrameBuffer* frameBuffer);

    // PBR
    std::shared_ptr<Shader> shaderPBR;
    void renderPBR(FrameBuffer* frameBuffer);

    // Normal Visualization
    std::shared_ptr<Shader> shaderNormalVisualization;
    void renderNormalVisualization(FrameBuffer* frameBuffer);

    // Bloom
    std::shared_ptr<Shader> shaderBloom;
    void renderBloom(FrameBuffer* frameBuffer0, FrameBuffer* frameBuffer1);
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;

    // G-Buffer
    std::shared_ptr<Shader> shaderGeometry;
    void renderGeometry(FrameBuffer* frameBuffer);

    // SSAO
    std::shared_ptr<Shader> shaderSSAO;
    void renderSSAO(FrameBuffer* frameBuffer);
    SSAO ssao = SSAO();

    std::shared_ptr<Shader> shaderSSAOBlur;
    void renderSSAOBlur(FrameBuffer* frameBuffer);

    // Deferred lighting
    std::shared_ptr<Shader> shaderDeferredLighting;
    void renderDeferredLighting(FrameBuffer* frameBuffer);

    // Post Processing
    std::shared_ptr<Shader> shaderPostProcessing;
    void renderPostProcessing(FrameBuffer* frameBuffer);
};

#endif // RENDERER_H

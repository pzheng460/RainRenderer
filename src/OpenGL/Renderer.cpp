#include "Renderer.h"
#include "shaderSetting.h"
#include "Common.h"

void Renderer::init() {
    for (auto& shadowMapFrameBuffer : shadowMapFrameBuffers) {
        shadowMapFrameBuffer.generateFrameBuffer(SHADOW_WIDTH, SHADOW_HEIGHT);
    }
    mainMSAAFrameBuffer.generateFrameBuffer(width, height);
    intermediateFrameBuffer.generateFrameBuffer(width, height);
    for (int i = 0; i < 2; ++i) {
        pingPongFrameBuffers[i].generateFrameBuffer(width, height);
    }
    gFrameBuffer.generateFrameBuffer(width, height);
    ssao.reset(width, height);
}

void Renderer::reset() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glViewport(0, 0, width, height);
}

void Renderer::draw(Scene& scene) {
    MSAA();
    faceCulling();
    gammaCorrection();

    // render shadow map 渲染阴影贴图
    for (int i = 0; i < scene.lights.size(); ++i) {
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        shadowMapFrameBuffers[i].bind();
        glClear(GL_DEPTH_BUFFER_BIT);
        shadowMapShaderSetting(simpleDepthShader, scene.lights[i]);
        scene.draw(simpleDepthShader, gui->IsFloorActive());
        shadowMapFrameBuffers[i].unbind();
    }

//    // debug depth map 调试深度贴图
//    reset();
//    Shader debugDepthQuad(FileSystem::getPath("src/OpenGL/shaders/debug_quad_depth.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/debug_quad_depth.fs").c_str());
//    debugDepthQuadShaderSetting(debugDepthQuad, shadowMapFrameBuffers[0].textureDepthBuffer->textureID);
//    Model screenQuadModel(GeometryType::QUAD);
//    Object screenQuadDebug(screenQuadModel);
//    screenQuadDebug.draw(debugDepthQuad);

    if (gui->getRenderingPath() == RenderingPath::FORWARD_RENDERING) {
        forwardRendering(scene);
    } else if (gui->getRenderingPath() == RenderingPath::DEFERRED_RENDERING) {
        deferredRendering(scene);
    }
}

void Renderer::forwardRendering(Scene& scene) {
    mainMSAAFrameBuffer.bind();
    reset();
    // render lights 渲染光源
    if (gui->IsLightActive()) {
        for (int i = 0; i < scene.lights.size(); ++i) {
            scene.lights[i].draw();
        }
    }
    // render scene 渲染场景
    if (gui->getMode() == RenderMode::BASIC) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/model_loading.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/model_loading.fs").c_str());
    } else if (gui->getMode() == RenderMode::PHONG) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/phong.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/phong.fs").c_str());
        phongShaderSetting(mainShader, gui->getCamera(), scene.lights, shadowMapFrameBuffers);
    } else if (gui->getMode() == RenderMode::BLINN_PHONG) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/blinn_phong.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/blinn_phong.fs").c_str());
        phongShaderSetting(mainShader, gui->getCamera(), scene.lights, shadowMapFrameBuffers, gui->IsShadowActive());
    } else if (gui->getMode() == RenderMode::DEPTH) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/depth_testing.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/depth_testing.fs").c_str());
    } else if (gui->getMode() == RenderMode::ENVIRONMENT_MAPPING) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/environment_mapping.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/environment_mapping.fs").c_str());
        phongShaderSetting(mainShader, gui->getCamera(), scene.lights, shadowMapFrameBuffers);
    } else if (gui->getMode() == RenderMode::PBR) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/pbr.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/pbr.fs").c_str());
        PBRShaderSetting(mainShader, scene.lights, scene.skybox.getIrradianceMap(), scene.skybox.getPrefilterMap(), scene.skybox.getBRDFLUTTexture());
    }
    scene.draw(mainShader, gui->IsFloorActive(), true);
    // render normal visualization 渲染法线可视化
    if (gui->IsNormalVisualizationActive()) {
        Shader normalShader(FileSystem::getPath("src/OpenGL/shaders/normal_visualization.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/normal_visualization.fs").c_str(), FileSystem::getPath("src/OpenGL/shaders/normal_visualization.gs").c_str());
        scene.draw(normalShader);
    }
    // render skybox 渲染天空盒，放在最后渲染保证early-z测试
    if (gui->IsSkyBoxActive()) {
        if (gui->getSkyboxLoadMode() == SkyboxLoadMode::CUBEMAP) {
            scene.skybox.drawGeometry();
        } else if (gui->getSkyboxLoadMode() == SkyboxLoadMode::SPHEREMAP) {
            scene.skybox.draw();
        }
    }
    mainMSAAFrameBuffer.unbind();

    mainMSAAFrameBuffer.transferFrameBuffer(intermediateFrameBuffer);

    if (gui->IsBloomActive()) {
        gaussianBlur();
    }

    reset();
    finalShaderSetting(finalShader, intermediateFrameBuffer.textureColorBuffers[0]->textureID, gui->IsHDRActive(), 10.0f, pingPongFrameBuffers[!horizontal].textureColorBuffers[0]->textureID, gui->IsBloomActive());
    Model screenQuadModel(GeometryType::QUAD);
    Object screenQuad(screenQuadModel);
    screenQuad.draw(finalShader);
}

void Renderer::deferredRendering(Scene &scene) {
    gFrameBuffer.bind();
    reset();
    // render lights 渲染光源
    if (gui->IsLightActive()) {
        for (int i = 0; i < scene.lights.size(); ++i) {
            scene.lights[i].draw();
        }
    }
    // render scene 渲染场景
    geometryBufferShaderSetting(gFrameBufferShader, false);
    scene.draw(gFrameBufferShader, gui->IsFloorActive(), true);
    // render skybox 渲染天空盒，放在最后渲染保证early-z测试
    if (gui->IsSkyBoxActive()) {
        if (gui->getSkyboxLoadMode() == SkyboxLoadMode::CUBEMAP) {
            scene.skybox.drawGeometry();
        } else if (gui->getSkyboxLoadMode() == SkyboxLoadMode::SPHEREMAP) {
            scene.skybox.draw();
        }
    }
    gFrameBuffer.unbind();

    unsigned int ssaoColorBufferBlur;
    if (firstIteration || gui->IsSSAOActive()) {
        ssaoColorBufferBlur = ssao.draw();
    }

    if (firstIteration) firstIteration = false;
    deferredLightingShaderSetting(deferredLightingShader,
                                  gFrameBuffer.textureColorBuffers[0]->textureID,
                                  gFrameBuffer.textureColorBuffers[1]->textureID,
                                  gFrameBuffer.textureColorBuffers[2]->textureID,
                                  ssaoColorBufferBlur,
                                  gui->getCamera(),
                                  scene.lights,
                                  gui->IsSSAOActive());
    Model screenQuadModel(GeometryType::QUAD);
    Object screenQuad(screenQuadModel);
    screenQuad.draw(deferredLightingShader);
}

void Renderer::gaussianBlur() {
    first_iteration = true;
    shaderBlur.use();
    shaderBlur.setInt("image", 0);

    for (unsigned int i = 0; i < amount; i++)
    {
        pingPongFrameBuffers[horizontal].bind();
        shaderBlur.setInt("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? intermediateFrameBuffer.textureColorBuffers[1]->textureID : pingPongFrameBuffers[!horizontal].textureColorBuffers[0]->textureID);  // bind texture of other framebuffer (or scene if first iteration)
        Model screenQuadModel(GeometryType::QUAD);
        Object screenQuad(screenQuadModel);
        screenQuad.draw(shaderBlur);
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
        pingPongFrameBuffers[horizontal].unbind();
    }
}

void Renderer::faceCulling() const {
    if (gui->IsFaceCullingActive()) {
        glEnable(GL_CULL_FACE); // enable face culling 启用面剔除
        glCullFace(GL_BACK); // cull back face 剔除背面
        glFrontFace(GL_CCW); // GL_CCW for counter clock-wise 逆时针
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void Renderer::MSAA() const {
    if (gui->IsMSAAActive()) {
        glEnable(GL_MULTISAMPLE);
    } else {
        glDisable(GL_MULTISAMPLE);
    }
}

void Renderer::gammaCorrection() const {
    if (gui->IsGammaCorrectionActive()) {
        glEnable(GL_FRAMEBUFFER_SRGB);
    } else {
        glDisable(GL_FRAMEBUFFER_SRGB);
    }
}
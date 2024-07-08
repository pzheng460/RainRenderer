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
        shadowMapShaderSetting(ShaderShadowMap, scene.lights[i]);
        scene.draw(ShaderShadowMap, gui->IsFloorActive());
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

    if (gui->IsBloomActive()) {
        gaussianBlur();
    }

    reset();
    finalShaderSetting(finalShader, intermediateFrameBuffer.textureColorBuffers[0]->textureID, gui->IsHDRActive(), 10.0f, pingPongFrameBuffers[!horizontal].textureColorBuffers[0]->textureID, gui->IsBloomActive());
    Model screenQuadModel(GeometryType::QUAD);
    Object screenQuad(screenQuadModel);
    screenQuad.draw(finalShader);
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
        mainShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_BASIC);
    } else if (gui->getMode() == RenderMode::PHONG) {
        mainShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_PHONG);
        phongShaderSetting(mainShader, gui->getCamera(), scene.lights, shadowMapFrameBuffers);
    } else if (gui->getMode() == RenderMode::BLINN_PHONG) {
        mainShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_BLINN_PHONG);
        phongShaderSetting(mainShader, gui->getCamera(), scene.lights, shadowMapFrameBuffers, gui->IsShadowActive());
    } else if (gui->getMode() == RenderMode::DEPTH) {
        mainShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_DEPTH_TESTING);
    } else if (gui->getMode() == RenderMode::ENVIRONMENT_MAPPING) {
        mainShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_ENVIRONMENT_MAPPING);
        phongShaderSetting(mainShader, gui->getCamera(), scene.lights, shadowMapFrameBuffers);
    } else if (gui->getMode() == RenderMode::PBR) {
        mainShader = ShaderFactory::createShader(ShaderFactoryType::SHADER_PBR);
        PBRShaderSetting(mainShader, scene.lights, scene.skybox.getIrradianceMap(), scene.skybox.getPrefilterMap(), scene.skybox.getBRDFLUTTexture());
    }
    scene.draw(mainShader, gui->IsFloorActive(), true);
    // render normal visualization 渲染法线可视化
    if (gui->IsNormalVisualizationActive()) {
        scene.draw(shaderNormalVisualization);
    }
    // render skybox 渲染天空盒，放在最后渲染保证early-z测试
    if (gui->IsSkyBoxActive()) {
        if (gui->getSkyboxLoadMode() == SkyboxLoadMode::CUBE_MAP) {
            scene.skybox.drawGeometry();
        } else if (gui->getSkyboxLoadMode() == SkyboxLoadMode::SPHERE_MAP) {
            scene.skybox.draw();
        }
    }
    mainMSAAFrameBuffer.unbind();

    mainMSAAFrameBuffer.transferFrameBuffer(intermediateFrameBuffer);
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
        if (gui->getSkyboxLoadMode() == SkyboxLoadMode::CUBE_MAP) {
            scene.skybox.drawGeometry();
        } else if (gui->getSkyboxLoadMode() == SkyboxLoadMode::SPHERE_MAP) {
            scene.skybox.draw();
        }
    }
    gFrameBuffer.unbind();

    unsigned int ssaoColorBufferBlur;
    if (firstIteration || gui->IsSSAOActive()) {
        ssaoColorBufferBlur = ssao.draw();
    }

    ssao.getSSAOBlurFrameBuffer().transferFrameBuffer(intermediateFrameBuffer);
    intermediateFrameBuffer.bind();
    reset();

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

    intermediateFrameBuffer.unbind();
}

void Renderer::gaussianBlur() {
    first_iteration = true;
    shaderBloom.use();
    shaderBloom.setInt("image", 0);

    for (unsigned int i = 0; i < amount; i++)
    {
        pingPongFrameBuffers[horizontal].bind();
        shaderBloom.setInt("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? intermediateFrameBuffer.textureColorBuffers[1]->textureID : pingPongFrameBuffers[!horizontal].textureColorBuffers[0]->textureID);  // bind texture of other framebuffer (or scene if first iteration)
        Model screenQuadModel(GeometryType::QUAD);
        Object screenQuad(screenQuadModel);
        screenQuad.draw(shaderBloom);
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
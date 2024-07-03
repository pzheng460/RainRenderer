#include "Renderer.h"
#include "render_implicit_geometry.h"
#include "shaderSetting.h"

void Renderer::init(Scene& scene) {
    for (int i = 0; i < scene.lights.size(); ++i) {
        shadowMapFrameBuffers.emplace_back(0, 1, 0);
        shadowMapFrameBuffers.back().generateFrameBuffer(SHADOW_WIDTH, SHADOW_HEIGHT);
    }

    mainMSAAFrameBuffer.generateFrameBuffer(SCR_WIDTH, SCR_HEIGHT);
    intermediateFrameBuffer.generateFrameBuffer(SCR_WIDTH, SCR_HEIGHT);
    for (int i = 0; i < 2; ++i) {
        pingPongFrameBuffers[i].generateFrameBuffer(SCR_WIDTH, SCR_HEIGHT);
    }
    gFrameBuffer.generateFrameBuffer(SCR_WIDTH, SCR_HEIGHT);
    ssao.reset(SCR_WIDTH, SCR_HEIGHT);
}

void Renderer::reset() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
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

    // debug depth map 调试深度贴图
//        reset();
//        Shader debugDepthQuad(FileSystem::getPath("src/OpenGL/shaders/debug_quad_depth.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/debug_quad_depth.fs").c_str());
//        debugDepthQuadShaderSetting(debugDepthQuad, scene.shadowMaps[0].getDepthMap());
//        renderQuad();

    if (gui->getRenderingPath() == FORWARDRENDERING) {
        forwardRendering(scene);
    } else if (gui->getRenderingPath() == DEFERREDRENDERING) {
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
    if (gui->getMode() == BASIC) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/model_loading.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/model_loading.fs").c_str());
    } else if (gui->getMode() == PHONG) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/phong.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/phong.fs").c_str());
        phongShaderSetting(mainShader, gui->getCamera(), scene.lights, shadowMapFrameBuffers);
    } else if (gui->getMode() == BLINNPHONG) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/blinn_phong.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/blinn_phong.fs").c_str());
        phongShaderSetting(mainShader, gui->getCamera(), scene.lights, shadowMapFrameBuffers, gui->IsShadowActive());
    } else if (gui->getMode() == DEPTH) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/depth_testing.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/depth_testing.fs").c_str());
    } else if (gui->getMode() == ENVIRONMENTMAPPING) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/environment_mapping.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/environment_mapping.fs").c_str());
        phongShaderSetting(mainShader, gui->getCamera(), scene.lights, shadowMapFrameBuffers);
    } else if (gui->getMode() == PBR) {
        mainShader = Shader(FileSystem::getPath("src/OpenGL/shaders/pbr.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/pbr.fs").c_str());
        PBRShaderSetting(mainShader, scene.lights, scene.skybox.getIrradianceMap(), scene.skybox.getPrefilterMap(), scene.skybox.getBRDFLUTTexture());
    }
    scene.draw(mainShader, gui->IsFloorActive());
    // render normal visualization 渲染法线可视化
    if (gui->IsNormalVisualizationActive()) {
        Shader normalShader(FileSystem::getPath("src/OpenGL/shaders/normal_visualization.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/normal_visualization.fs").c_str(), FileSystem::getPath("src/OpenGL/shaders/normal_visualization.gs").c_str());
        scene.draw(normalShader);
    }
    // render skybox 渲染天空盒，放在最后渲染保证early-z测试
    if (gui->IsSkyBoxActive()) {
        if (gui->getSkyboxLoadMode() == CUBEMAP) {
            scene.skybox.drawGeometry();
        } else if (gui->getSkyboxLoadMode() == SPHEREMAP) {
            scene.skybox.draw();
        }
    }
    mainMSAAFrameBuffer.unbind();

    mainMSAAFrameBuffer.transferFrameBuffer(intermediateFrameBuffer, SCR_WIDTH, SCR_HEIGHT);

    if (gui->IsBloomActive()) {
        gaussianBlur();
    }

    reset();
    finalShaderSetting(finalShader, intermediateFrameBuffer.getTextureColorBuffer()[0]->getTexture(), gui->IsHDRActive(), 10.0f, pingPongFrameBuffers[!horizontal].getTextureColorBuffer()[0]->getTexture(), gui->IsBloomActive());
    renderQuad();
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
    scene.draw(gFrameBufferShader, gui->IsFloorActive());
    // render skybox 渲染天空盒，放在最后渲染保证early-z测试
    if (gui->IsSkyBoxActive()) {
        if (gui->getSkyboxLoadMode() == CUBEMAP) {
            scene.skybox.drawGeometry();
        } else if (gui->getSkyboxLoadMode() == SPHEREMAP) {
            scene.skybox.draw();
        }
    }
    gFrameBuffer.unbind();

    unsigned int ssaoColorBufferBlur;
    if (firstIteration || gui->IsSSAOActive()) {
        ssaoColorBufferBlur = ssao.draw();
    }

    if (firstIteration) firstIteration = false;
    deferredLightingShaderSetting(deferredLightingShader, gFrameBuffer.getTextureColorBuffer()[0]->getTexture(), gFrameBuffer.getTextureColorBuffer()[1]->getTexture(), gFrameBuffer.getTextureColorBuffer()[2]->getTexture(), ssaoColorBufferBlur, gui->getCamera(), scene.lights, gui->IsSSAOActive());
    renderQuad();
}

void Renderer::gaussianBlur() {
    first_iteration = true;
    shaderBlur.use();
    shaderBlur.setInt("image", 0);

    for (unsigned int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingPongFrameBuffers[horizontal].getFrameBuffer());
        shaderBlur.setInt("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? intermediateFrameBuffer.getTextureColorBuffer()[1]->getTexture() : pingPongFrameBuffers[!horizontal].getTextureColorBuffer()[0]->getTexture());  // bind texture of other framebuffer (or scene if first iteration)
        renderQuad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
#include "Renderer.h"
#include "Common.h"

Renderer::Renderer(int width, int height, GUI& gui, Scene& scene)
        : gui(gui)
        , width(width)
        , height(height)
        , scene(scene) {
    auto frameBufferDefaultPtr = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_DEFAULT);
    frameBufferDefault = std::shared_ptr<FrameBuffer>(frameBufferDefaultPtr);
    auto frameBufferMSAAPtr = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_MSAA);
    frameBufferMSAA = std::shared_ptr<FrameBuffer>(frameBufferMSAAPtr);
    auto frameBufferIntermediatePtr = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_INTERMEDIATE);
    frameBufferIntermediate = std::shared_ptr<FrameBuffer>(frameBufferIntermediatePtr);
    auto frameBufferGeometryPtr = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_GEOMETRY);
    frameBufferGeometry = std::shared_ptr<FrameBuffer>(frameBufferGeometryPtr);
    auto frameBufferSSAOPtr = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_SSAO);
    frameBufferSSAO = std::shared_ptr<FrameBuffer>(frameBufferSSAOPtr);
    auto frameBufferSSAOBlurPtr = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_SSAO_BLUR);
    frameBufferSSAOBlur = std::shared_ptr<FrameBuffer>(frameBufferSSAOBlurPtr);
    for (int i = 0; i < scene.lights.size(); ++i) {
        frameBufferShadowMaps.emplace_back(FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_SHADOW_MAP));
    }
    for (int i = 0; i < 2; ++i) {
        frameBufferBlooms.emplace_back(FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_PING_PONG));
    }

    auto shaderLightPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_LIGHT);
    shaderLight = std::shared_ptr<Shader>(shaderLightPtr);
    auto shaderSkyboxCubeMapPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_SKYBOX_CUBE_MAP);
    shaderSkyboxCubeMap = std::shared_ptr<Shader>(shaderSkyboxCubeMapPtr);
    auto shaderSkyboxSphereMapPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_SKYBOX_SPHERE_MAP);
    shaderSkyboxSphereMap = std::shared_ptr<Shader>(shaderSkyboxSphereMapPtr);
    auto shaderSkyboxSphereMapToCubeMapPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_SPHERE_MAP_TO_CUBE_MAP);
    shaderSkyboxSphereMapToCubeMap = std::shared_ptr<Shader>(shaderSkyboxSphereMapToCubeMapPtr);
    auto shaderIrradiancePtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_IRRADIANCE_MAP);
    shaderIrradiance = std::shared_ptr<Shader>(shaderIrradiancePtr);
    auto shaderPrefilterPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_PREFILTER_MAP);
    shaderPrefilter = std::shared_ptr<Shader>(shaderPrefilterPtr);
    auto shaderBRDFLUTPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_BRDF_LUT);
    shaderBRDFLUT = std::shared_ptr<Shader>(shaderBRDFLUTPtr);
    auto shaderShadowMapPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_SHADOW_MAP);
    shaderShadowMap = std::shared_ptr<Shader>(shaderShadowMapPtr);
    auto shaderShadowMapDebugPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_SHADOW_MAP_DEBUG);
    shaderShadowMapDebug = std::shared_ptr<Shader>(shaderShadowMapDebugPtr);
    auto shaderBasicPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_BASIC);
    shaderBasic = std::shared_ptr<Shader>(shaderBasicPtr);
    auto shaderPhongPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_PHONG);
    shaderPhong = std::shared_ptr<Shader>(shaderPhongPtr);
    auto shaderBlinnPhongPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_BLINN_PHONG);
    shaderBlinnPhong = std::shared_ptr<Shader>(shaderBlinnPhongPtr);
    auto shaderNormalVisualizationPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_NORMAL_VISUALIZATION);
    shaderNormalVisualization = std::shared_ptr<Shader>(shaderNormalVisualizationPtr);
    auto shaderBloomPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_BLOOM);
    shaderBloom = std::shared_ptr<Shader>(shaderBloomPtr);
    auto shaderGeometryPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_GEOMETRY);
    shaderGeometry = std::shared_ptr<Shader>(shaderGeometryPtr);
    auto shaderSSAOPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_SSAO);
    shaderSSAO = std::shared_ptr<Shader>(shaderSSAOPtr);
    auto shaderSSAOBlurPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_SSAO_BLUR);
    shaderSSAOBlur = std::shared_ptr<Shader>(shaderSSAOBlurPtr);
    auto shaderDeferredLightingPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_DEFERRED_LIGHTING);
    shaderDeferredLighting = std::shared_ptr<Shader>(shaderDeferredLightingPtr);
    auto shaderPostProcessingPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_POST_PROCESSING);
    shaderPostProcessing = std::shared_ptr<Shader>(shaderPostProcessingPtr);

    Model screenQuadModel(GeometryType::QUAD);
    screenQuad = std::make_unique<Object>(screenQuadModel);

    setSize(width, height);
    init();
}

void Renderer::setSize(int newWidth, int newHeight) {
    this->width = newWidth;
    this->height = newHeight;

    frameBufferDefault->width = width;
    frameBufferDefault->height = height;
    for (auto& shadowMapFrameBuffer : frameBufferShadowMaps) {
        shadowMapFrameBuffer->setSize(SHADOW_WIDTH, SHADOW_HEIGHT);
    }
    frameBufferMSAA->setSize(width, height);
    frameBufferIntermediate->setSize(width, height);
    for (int i = 0; i < 2; ++i) {
        frameBufferBlooms[i]->setSize(width, height);
    }
    frameBufferGeometry->setSize(width, height);
    frameBufferSSAO->setSize(width, height);
    frameBufferSSAOBlur->setSize(width, height);
}

void Renderer::init() {
    for (auto& frameBufferShadowMap : frameBufferShadowMaps) {
        frameBufferShadowMap->init();
    }
    frameBufferMSAA->init();
    frameBufferIntermediate->init();
    for (int i = 0; i < 2; ++i) {
        frameBufferBlooms[i]->init();
    }
    frameBufferGeometry->init();
    frameBufferSSAO->init();
    frameBufferSSAOBlur->init();
}

void Renderer::reset() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glViewport(0, 0, width, height);
}

void Renderer::draw() {
    MSAA();
    faceCulling();
    gammaCorrection();

    renderShadowMap();

//    renderShadowMapDebug();

    if (gui.renderingPath == RenderingPath::FORWARD_RENDERING) {
        forwardRendering();
    } else if (gui.renderingPath == RenderingPath::DEFERRED_RENDERING) {
        deferredRendering();
    }

    if (gui.bloomActive) {
        renderBloom();
    }

    renderPostPossing();
}

void Renderer::forwardRendering() {
    // render scene 渲染场景
    if (gui.mode == RenderMode::BASIC)
    {
        renderBasic();
    }
    else if (gui.mode == RenderMode::PHONG)
    {
        renderPhong();
    }
    else if (gui.mode == RenderMode::BLINN_PHONG)
    {
        renderBlinnPhong();
    }
    else if (gui.mode == RenderMode::DEPTH)
    {
        renderDepthTesting();
    }
    else if (gui.mode == RenderMode::ENVIRONMENT_MAPPING)
    {
        renderEnvironmentMapping();
    }
    else if (gui.mode == RenderMode::PBR)
    {
        renderPBR();
    }
    else
    {
        std::cout << "Unknown render mode! Change to Basic mode" << std::endl;
        renderBasic();
    }
    frameBufferMSAA->bind();
    // render lights 渲染光源
    if (gui.lightActive) {
        renderLight();
    }
    frameBufferMSAA->unbind();

    // render normal visualization 渲染法线可视化
    if (gui.normalVisualizationActive) {
        renderNormalVisualization();
    }

//    // render skybox 渲染天空盒，放在最后渲染保证early-z测试
//    frameBufferMSAA->bind();
//    if (gui.skyBoxActive) {
//        if (gui.skyboxMode == SkyboxLoadMode::CUBE_MAP) {
//            scene.skybox.drawGeometry();
//        } else if (gui.skyboxMode == SkyboxLoadMode::SPHERE_MAP) {
//            scene.skybox.draw();
//        }
//    }
//    frameBufferMSAA->unbind();

    for (int i = 0; i < frameBufferIntermediate->numOfColorTextureAttachments; ++i) {
        frameBufferMSAA->transferFrameBuffer(*frameBufferIntermediate, GL_COLOR_BUFFER_BIT, i, i);
    }
}

void Renderer::deferredRendering() {
    renderGeometry();

    if (gui.SSAOActive) {
        renderSSAO();
        renderSSAOBlur();
    }

    renderDeferredLighting();

    frameBufferGeometry->transferFrameBuffer(*frameBufferIntermediate, GL_DEPTH_BUFFER_BIT);

    frameBufferIntermediate->bind();
    // render lights 渲染光源
    if (gui.lightActive) {
        renderLight();
    }
//    // render skybox 渲染天空盒，放在最后渲染保证early-z测试
//    if (gui.skyBoxActive) {
//        if (gui.skyboxMode == SkyboxLoadMode::CUBE_MAP) {
//            scene.skybox.drawGeometry();
//        } else if (gui.skyboxMode == SkyboxLoadMode::SPHERE_MAP) {
//            scene.skybox.draw();
//        }
//    }
    frameBufferIntermediate->unbind();
}

void Renderer::faceCulling() const {
    if (gui.faceCullingActive) {
        glEnable(GL_CULL_FACE); // enable face culling 启用面剔除
        glCullFace(GL_BACK); // cull back face 剔除背面
        glFrontFace(GL_CCW); // GL_CCW for counter clock-wise 逆时针
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void Renderer::MSAA() const {
    if (gui.MSAAActive) {
        glEnable(GL_MULTISAMPLE);
    } else {
        glDisable(GL_MULTISAMPLE);
    }
}

void Renderer::gammaCorrection() const {
    if (gui.gammaCorrectionActive) {
        glEnable(GL_FRAMEBUFFER_SRGB);
    } else {
        glDisable(GL_FRAMEBUFFER_SRGB);
    }
}
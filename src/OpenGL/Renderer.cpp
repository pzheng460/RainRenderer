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
    auto frameBufferSkyboxCapturePtr = FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_SKYBOX_CAPTURE);
    frameBufferSkyboxCapture = std::shared_ptr<FrameBuffer>(frameBufferSkyboxCapturePtr);
    for (int i = 0; i < scene.lights.size(); ++i) {
        frameBufferShadowMaps.emplace_back(FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_SHADOW_MAP));
    }
    for (int i = 0; i < 2; ++i) {
        frameBufferBlooms.emplace_back(FrameBufferFactory::createFrameBuffer(FrameBufferFactoryType::FRAME_BUFFER_BLOOM));
    }

    auto shaderLightPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_LIGHT);
    shaderLight = std::shared_ptr<Shader>(shaderLightPtr);
    auto shaderSkyboxPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_SKYBOX);
    shaderSkybox = std::shared_ptr<Shader>(shaderSkyboxPtr);
    auto shaderSkyboxHDRPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_SKYBOX_HDR);
    shaderSkyboxHDR = std::shared_ptr<Shader>(shaderSkyboxHDRPtr);
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
    auto shaderDepthTestingPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_DEPTH_TESTING);
    shaderDepthTesting = std::shared_ptr<Shader>(shaderDepthTestingPtr);
    auto shaderEnvironmentMappingPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_ENVIRONMENT_MAPPING);
    shaderEnvironmentMapping = std::shared_ptr<Shader>(shaderEnvironmentMappingPtr);
    auto shaderPBRPtr = ShaderFactory::createShader(ShaderFactoryType::SHADER_PBR);
    shaderPBR = std::shared_ptr<Shader>(shaderPBRPtr);
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

    setAllSize(width, height);
    init();
}

void Renderer::setSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
}

void Renderer::setAllSize(int newWidth, int newHeight) {
    setSize(newWidth, newHeight);

    frameBufferDefault->setSize(newWidth, newHeight);
    for (auto& shadowMapFrameBuffer : frameBufferShadowMaps) {
        shadowMapFrameBuffer->setAllSizes(shadowWidth, shadowHeight);
    }
    frameBufferMSAA->setAllSizes(width, height);
    frameBufferIntermediate->setAllSizes(width, height);
    for (int i = 0; i < 2; ++i) {
        frameBufferBlooms[i]->setAllSizes(width, height);
    }
    frameBufferGeometry->setAllSizes(width, height);
    frameBufferSSAO->setAllSizes(width, height);
    frameBufferSSAOBlur->setAllSizes(width, height);
    frameBufferSkyboxCapture->setAllSizes(scene.skybox->width, scene.skybox->height);
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
    frameBufferSkyboxCapture->init();
}

void Renderer::draw() {
    // preprocessing 预处理
    if (!scene.skybox->initialized) {
        if (gui.skyboxMode == SkyboxLoadMode::SPHERE_MAP) {
            renderSkyboxSphereMapToCubeMap(frameBufferSkyboxCapture.get());
        }
        renderIrradiance(frameBufferSkyboxCapture.get());
        renderPrefilter(frameBufferSkyboxCapture.get());
        renderBRDFLUT(frameBufferSkyboxCapture.get());
        scene.skybox->initialized = true;
    }

    MSAA();
    faceCulling();
    gammaCorrection();

    for (int i = 0; i < scene.lights.size(); ++i) {
        renderShadowMap(frameBufferShadowMaps[i].get(), i);
    }

//    renderShadowMapDebug(frameBufferDefault.get(), 0;

    if (gui.renderingPath == RenderingPath::FORWARD_RENDERING) {
        forwardRendering();
    } else if (gui.renderingPath == RenderingPath::DEFERRED_RENDERING) {
        deferredRendering();
    }

    if (gui.bloomActive) {
        first_iteration = true;

        for (unsigned int i = 0; i < amount; ++i)
        {
            renderBloom(frameBufferBlooms[horizontal].get(), frameBufferBlooms[!horizontal].get());
        }
    }

    renderPostProcessing(frameBufferDefault.get());
}

void Renderer::forwardRendering() {
    // render scene 渲染场景
    if (gui.mode == RenderMode::BASIC)
    {
        renderBasic(frameBufferMSAA.get());
    }
    else if (gui.mode == RenderMode::PHONG)
    {
        renderPhong(frameBufferMSAA.get());
    }
    else if (gui.mode == RenderMode::BLINN_PHONG)
    {
        renderBlinnPhong(frameBufferMSAA.get());
    }
    else if (gui.mode == RenderMode::DEPTH)
    {
        renderDepthTesting(frameBufferMSAA.get());
    }
    else if (gui.mode == RenderMode::ENVIRONMENT_MAPPING)
    {
        renderEnvironmentMapping(frameBufferMSAA.get());
    }
    else if (gui.mode == RenderMode::PBR)
    {
        renderPBR(frameBufferMSAA.get());
    }
    else
    {
        std::cout << "Unknown render mode! Change to Basic mode" << std::endl;
        renderBasic(frameBufferMSAA.get());
    }

    // render lights 渲染光源
    if (gui.lightActive) {
        renderLight(frameBufferMSAA.get());
    }

    // render normal visualization 渲染法线可视化
    if (gui.normalVisualizationActive) {
        renderNormalVisualization(frameBufferMSAA.get());
    }

    // render skybox 渲染天空盒，放在最后渲染保证early-z测试
    if (gui.skyBoxActive) {
        if (gui.skyboxMode == SkyboxLoadMode::CUBE_MAP)
        {
            renderSkybox(frameBufferMSAA.get());
        }
        else if (gui.skyboxMode == SkyboxLoadMode::SPHERE_MAP)
        {
            renderSkyboxHDR(frameBufferMSAA.get());
        }
    }

    for (int i = 0; i < frameBufferIntermediate->numOfColorTextureAttachments; ++i) {
        frameBufferMSAA->transferFrameBuffer(*frameBufferIntermediate, GL_COLOR_BUFFER_BIT, i, i);
    }
}

void Renderer::deferredRendering() {
    renderGeometry(frameBufferGeometry.get());

    if (gui.SSAOActive) {
        renderSSAO(frameBufferSSAO.get());
        renderSSAOBlur(frameBufferSSAOBlur.get());
    }

    renderDeferredLighting(frameBufferIntermediate.get());

    frameBufferGeometry->transferFrameBuffer(*frameBufferIntermediate, GL_DEPTH_BUFFER_BIT);

    // render lights 渲染光源
    if (gui.lightActive) {
        renderLight(frameBufferIntermediate.get());
    }
    // render skybox 渲染天空盒，放在最后渲染保证early-z测试
    if (gui.skyBoxActive) {
        if (gui.skyboxMode == SkyboxLoadMode::CUBE_MAP)
        {
            renderSkybox(frameBufferIntermediate.get());
        }
        else if (gui.skyboxMode == SkyboxLoadMode::SPHERE_MAP)
        {
            renderSkyboxHDR(frameBufferIntermediate.get());
        }
    }
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
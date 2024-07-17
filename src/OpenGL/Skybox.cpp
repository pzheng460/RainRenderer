#include "Skybox.h"

Skybox::Skybox(const std::vector<std::string>& paths) {
    model = Model(GeometryType::CUBE);
    cubeMap = std::shared_ptr<Texture>(loadTextureCube(paths));
    init();
}

Skybox::Skybox(const std::string& path) {
    model = Model(GeometryType::CUBE);
    sphereMap = std::shared_ptr<Texture>(loadTexture2DHDR(path));
    cubeMap = std::shared_ptr<Texture>(TextureFactory::createTexture(TextureFactoryType::TEXTURE_CUBE_MAP_SKYBOX));
    init();
}

void Skybox::init() {
    irradiance = std::shared_ptr<Texture>(TextureFactory::createTexture(TextureFactoryType::TEXTURE_IRRADIANCE));
    prefilter = std::shared_ptr<Texture>(TextureFactory::createTexture(TextureFactoryType::TEXTURE_PREFILTER));
    brdfLUT = std::shared_ptr<Texture>(TextureFactory::createTexture(TextureFactoryType::TEXTURE_BRDF_LUT));
}

//void Skybox::convertSphereMapToCubeMap(unsigned int hdrTexture) {
//    // pbr: setup cubemap to render to and attach to framebuffer
//    // ---------------------------------------------------------
//    glGenTextures(1, &envCubemap);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
//    for (unsigned int i = 0; i < 6; i++)
//    {
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
//    }
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    // pbr: convert HDR equirectangular environment map to cubemap equivalent
//    // ----------------------------------------------------------------------
//    equirectangularToCubemapShader.use();
//    equirectangularToCubemapShader.setInt("equirectangularMap", 0);
//    equirectangularToCubemapShader.setMat4("projection", captureProjection);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, hdrTexture);
//
//    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    for (unsigned int i = 0; i < 6; ++i)
//    {
//        equirectangularToCubemapShader.setMat4("view", captureViews[i]);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        renderCube();
//    }
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
//    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
//    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
//}
//
//void Skybox::draw() {
//    glDepthFunc(GL_LEQUAL);
//    skyboxShader.use();
//
//    skyboxShader.setInt("environmentMap", 0);
//
//    // MVP matrices
//    skyboxShader.setMat4("view", viewMatrix);
//    skyboxShader.setMat4("projection", projectionMatrix);
//
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
//    renderCube();
//    glDepthFunc(GL_LESS); // set depth function back to default
//}

//void Skybox::createIrradianceMap() {
//    // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
//    // --------------------------------------------------------------------------------
//    glGenTextures(1, &irradianceMap);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
//    for (unsigned int i = 0; i < 6; ++i)
//    {
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
//    }
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
//
//    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
//    // -----------------------------------------------------------------------------
//    irradianceShader.use();
//    irradianceShader.setInt("environmentMap", 0);
//    irradianceShader.setMat4("projection", captureProjection);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
//
//    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    for (unsigned int i = 0; i < 6; ++i)
//    {
//        irradianceShader.setMat4("view", captureViews[i]);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        renderCube();
//    }
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//
//void Skybox::createPrefilterMap() {
//    // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
//    // --------------------------------------------------------------------------------
//    glGenTextures(1, &prefilterMap);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
//    for (unsigned int i = 0; i < 6; ++i)
//    {
//        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
//    }
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
//    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
//
//    // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
//    // ----------------------------------------------------------------------------------------------------
//    prefilterShader.use();
//    prefilterShader.setInt("environmentMap", 0);
//    prefilterShader.setMat4("projection", captureProjection);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
//
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    unsigned int maxMipLevels = 5;
//    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
//    {
//        // reisze framebuffer according to mip-level size.
//        unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
//        unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
//        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
//        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
//        glViewport(0, 0, mipWidth, mipHeight);
//
//        float roughness = (float)mip / (float)(maxMipLevels - 1);
//        prefilterShader.setFloat("roughness", roughness);
//        for (unsigned int i = 0; i < 6; ++i)
//        {
//            prefilterShader.setMat4("view", captureViews[i]);
//            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);
//
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//            renderCube();
//        }
//    }
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//// pbr: generate a 2D LUT from the BRDF equations used.
//// ----------------------------------------------------
//void Skybox::generateBRDFLUTTexture() {
//    glGenTextures(1, &brdfLUTTexture);
//
//    // pre-allocate enough memory for the LUT texture.
//    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
//    // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
//    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
//    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);
//
//    glViewport(0, 0, 512, 512);
//    brdfShader.use();
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    renderQuad();
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

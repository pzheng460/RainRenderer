#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <utils/filesystem.h>
#include <vector>
#include <string>

class Skybox {
public:
    Skybox() = default;
    Skybox(const Shader& shader, const std::vector<std::string>& faces);
    Skybox(const Shader& shader, const std::string& hdrPath);

    void setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);
    void draw();
    void drawGeometry();

    unsigned int getIrradianceMap() const {
      return irradianceMap;
    }

    unsigned int getPrefilterMap() const {
      return prefilterMap;
    }

    unsigned int getBRDFLUTTexture() const {
      return brdfLUTTexture;
    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix {};
    glm::mat4 projectionMatrix {};

private:
    unsigned int cubemapTexture;

    Shader skyboxShader;
    Shader equirectangularToCubemapShader = Shader(FileSystem::getPath("src/OpenGL/shaders/cubemap.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/equirectangular_to_cubemap.fs").c_str());
    Shader irradianceShader = Shader(FileSystem::getPath("src/OpenGL/shaders/cubemap.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/irradiance_convolution.fs").c_str());
    Shader prefilterShader = Shader(FileSystem::getPath("src/OpenGL/shaders/cubemap.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/prefilter.fs").c_str());
    Shader brdfShader = Shader(FileSystem::getPath("src/OpenGL/shaders/brdf.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/brdf.fs").c_str());
    unsigned int envCubemap, irradianceMap, prefilterMap, brdfLUTTexture;
    glm::vec3 position;

    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[6] =
            {
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
            };

    unsigned int captureFBO, captureRBO;
    void loadCubemap(const std::vector<std::string>& faces);

    void setupFramebuffers();
    void loadSphereMap(const std::string& hdrPath);
    void convertSphereMapToCubeMap(unsigned int hdrTexture);
    void createIrradianceMap();
    void createPrefilterMap();
    void generateBRDFLUTTexture();
};

#endif // SKYBOX_H

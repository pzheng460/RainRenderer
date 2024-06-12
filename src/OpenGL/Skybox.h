#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <vector>
#include <string>

class Skybox {
public:
    Skybox(const Shader& shader, const std::vector<std::string>& faces);
    Skybox(const Shader& shader, const std::string& hdrPath);

    void setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);
    void drawSkybox();
    void drawObjectGeometry(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT);

    unsigned int getIrradianceMap() const;
    unsigned int getPrefilterMap() const;
    unsigned int getBRDFLUTTexture() const;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix {};
    glm::mat4 projectionMatrix {};

private:
    unsigned int cubemapTexture;

    Shader skyboxShader;
    Shader equirectangularToCubemapShader, irradianceShader, prefilterShader, brdfShader;
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
    void setupFramebuffers();
    void loadHDRtexture(const std::string& hdrPath);
    void convertHDRtoCubemap(unsigned int hdrTexture);
    void createIrradianceMap();
    void createPrefilterMap();
    void generateBRDFLUTTexture();
};

#endif // SKYBOX_H

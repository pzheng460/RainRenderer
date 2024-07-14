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
#include "Object.h"

class Skybox : public Object {
public:
    Skybox() = default;
    explicit Skybox(const std::vector<std::string>& faces);
    explicit Skybox(const std::string& hdrPath);

//    void draw();
//    void drawGeometry();

    unsigned int getCubemapTexture() const {
      return cubemapTexture;
    }

    unsigned int getIrradianceMap() const {
      return irradianceMap;
    }

    unsigned int getPrefilterMap() const {
      return prefilterMap;
    }

    unsigned int getBRDFLUTTexture() const {
      return brdfLUTTexture;
    }

private:
    unsigned int cubemapTexture;

    unsigned int envCubemap, irradianceMap, prefilterMap, brdfLUTTexture;

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
//    void loadCubemap(const std::vector<std::string>& faces);
//
//    void setupFramebuffers();
//    void loadSphereMap(const std::string& hdrPath);
//    void convertSphereMapToCubeMap(unsigned int hdrTexture);
//    void createIrradianceMap();
//    void createPrefilterMap();
//    void generateBRDFLUTTexture();
};

#endif // SKYBOX_H

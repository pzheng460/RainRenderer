#include "PBRRenderer.h"
#include <learnopengl/filesystem.h>
#include "render_implicit_geometry.h"

PBRRenderer::PBRRenderer(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, Scene& scene)
        : Renderer(SCR_WIDTH, SCR_HEIGHT, scene),
          pbrShader(FileSystem::getPath("src/OpenGL/shaders/pbr.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/pbr.fs").c_str()) {}

void PBRRenderer::initialize() {
    pbrShader.use();
    pbrShader.setInt("albedoMap", 0);
    pbrShader.setInt("normalMap", 1);
    pbrShader.setInt("metallicMap", 2);
    pbrShader.setInt("roughnessMap", 3);
    pbrShader.setInt("aoMap", 4);
    pbrShader.setInt("irradianceMap", 5);
    pbrShader.setInt("prefilterMap", 6);
    pbrShader.setInt("brdfLUT", 7);
}

void PBRRenderer::renderScene(GUI& gui) {
    if (gui.IsSkyBoxActive()) {
        scene.skybox.drawSkybox(scene.getCamera(), SCR_WIDTH, SCR_HEIGHT);
    }

    // render light source (simply re-render sphere at light positions)
    // this looks a bit off as we use the same shader, but it'll make their positions obvious and
    // keeps the codeprint small.
    for (unsigned int i = 0; i < scene.lights.size(); ++i)
    {
        glm::vec3 newPos = scene.lights[i].getPosition() + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        newPos = scene.lights[i].getPosition();
        pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
        pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", scene.lights[i].getColor());
    }

    // bind pre-computed IBL data
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene.getSkybox().getIrradianceMap());
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, scene.getSkybox().getPrefilterMap());
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, scene.getSkybox().getBRDFLUTTexture());

    for (auto& obj : scene.getObjects()) {
        if (obj->getRenderMode() == PBR) {
            pbrShader.use();
            pbrShader.setMat4("model", obj->getModelMatrix());
            pbrShader.setMat4("view", obj->getViewMatrix());
            pbrShader.setMat4("projection", obj->getProjectionMatrix());
            pbrShader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(obj->getModelMatrix()))));

            // bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, obj->getAlbedoMap());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, obj->getNormalMap());
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, obj->getMetallicMap());
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, obj->getRoughnessMap());
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, obj->getAOMap());

            // 使用 std::holds_alternative 和 std::get 进行类型检查和处理
            if (std::holds_alternative<Model>(obj->data)) {
                std::get<Model>(obj->data).Draw(pbrShader);
            } else if (std::holds_alternative<ImplicitGeometryType>(obj->data)) {
                renderGeometry(std::get<ImplicitGeometryType>(obj->data));
            }
        }
    }
}


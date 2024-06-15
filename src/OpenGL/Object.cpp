#include "Object.h"
#include "GeometryGenerator.h"

Object::Object(const Model& model, const Shader& shader)
        : model(model), shader(shader), geometryType(MODEL) {
}

Object::Object(ImplicitGeometryType geometryType, const Shader& shader, bool initializeTextures, std::string texturePath)
        : model(), shader(shader), geometryType(geometryType) {
    generateModel(geometryType);
    if (initializeTextures) {
        loadTextures(model.meshes[0].textures,
                     FileSystem::getPath(texturePath.c_str()));
    }
}

void Object::setMVP(Camera& camera, float SCR_WIDTH, float SCR_HEIGHT) {
    // MVP matrices
    modelMatrix = glm::translate(glm::mat4(1.0f), position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
    viewMatrix = camera.GetViewMatrix();
    // 参数：fov视野，aspect宽高比，zNear近平面，zFar远平面
    projectionMatrix = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
}

void Object::draw() {
    model.Draw(shader);
}

void Object::basicShaderSetting() {
    shader.use();
    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);
}

void Object::phongShaderSetting(Camera& camera, std::vector<Light>& lights) {
    shader.use();

    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        shader.setVec3("pointLights[" + std::to_string(i) + "].position", lights[i].getPosition());
        shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", lights[i].getAmbientColor());
        shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", lights[i].getDiffuseColor());
        shader.setVec3("pointLights[" + std::to_string(i) + "].specular", lights[i].getSpecularColor());

        shader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
        shader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
        shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
    }

    shader.setVec3("viewPos", camera.Position);
    shader.setFloat("material.shininess", 32.0f);

    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);
}

void Object::generateModel(ImplicitGeometryType geometryType) {
    switch (geometryType) {
        case SPHERE:
            model = std::move(GeometryGenerator::generateSphere());
            break;
        case CUBE:
            model = GeometryGenerator::generateCube();
            break;
        case QUAD:
            model = GeometryGenerator::generateQuad();
            break;
        case PLANE:
            model = GeometryGenerator::generatePlane();
            break;
        default:
            std::cerr << "Unknown geometry type" << std::endl;
            break;
    }
}

void Object::loadTextures(std::vector<Texture>& textures,
                                           const std::string& diffusePath) {
    std::cout << "Loading texture from path: " << diffusePath << std::endl;
    Texture texture = loadTexture(diffusePath, "texture_diffuse");
    if (texture.id != 0) { // 检查纹理是否加载成功
        textures.push_back(texture);
        std::cout << "Texture loaded successfully." << std::endl;
    } else {
        std::cerr << "Texture failed to load." << std::endl;
    }
}

Texture Object::loadTexture(const std::string& path, const std::string& typeName) {
    Texture texture;
    texture.id = TextureFromFile(path.c_str(), "");
    texture.type = typeName;
    texture.path = path.c_str();
    return texture;
}

#include "Object.h"

Object::Object(const AssimpModel::Model& model) : modelVariant(model) {}

Object::Object(GeometryType geometryType, bool initializeTextures, std::string texturePath, std::string name) {
    GeometryModel geometryModel;
    modelVariant = std::move(geometryModel);
    std::get<GeometryModel>(modelVariant).geometry = GeometryFactory::createGeometry(geometryType);
    std::get<GeometryModel>(modelVariant).geometry->createVAO();
    if (initializeTextures) {
        addTexture(name, FileSystem::getPath(texturePath).c_str());
    }
}

void Object::addTexture(std::string name, const char* path) {
    if (modelVariant.index() == 0) {
        std::cout << "Unable to add texture to model!\n" << std::endl;
    } else {
        AssimpModel::Texture texture;
        texture.id = loadTexture(path);
        texture.type = name;
        texture.path = path;
        std::get<GeometryModel>(modelVariant).textures.push_back(texture);
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

void Object::draw(Shader& shader, bool setTexture) {
    shader.use();
    shader.setMat4("model", modelMatrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);

    if (modelVariant.index() == 0) {
        std::get<AssimpModel::Model>(modelVariant).Draw(shader, setTexture);
//        if (setTexture) {
//            GLint diffuseLoc = glGetUniformLocation(shader.ID, "material.texture_diffuse1");
//            GLint specularLoc = glGetUniformLocation(shader.ID, "material.texture_specular1");
//            GLint shadowLoc = glGetUniformLocation(shader.ID, "shadowMaps[0]");
//            GLint value1, value2, value3;
//            glGetUniformiv(shader.ID, diffuseLoc, &value1);
//            glGetUniformiv(shader.ID, specularLoc, &value2);
//            glGetUniformiv(shader.ID, shadowLoc, &value3);
//            std::cout << "Diffuse: " << value1 << std::endl;
//            std::cout << "Specular: " << value2 << std::endl;
//            std::cout << "Shadow: " << value3 << std::endl;
//        }
    } else {
        if (setTexture) {
            for (int i = 0; i < std::get<GeometryModel>(modelVariant).textures.size(); ++i) {
                auto texture = std::get<GeometryModel>(modelVariant).textures[i];
                shader.setTexture(texture.type, texture.id);
            }
        }
        std::get<GeometryModel>(modelVariant).geometry->draw();
    }
}

unsigned int Object::loadTexture(const char *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

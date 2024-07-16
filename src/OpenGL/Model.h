#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Geometry.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

class Model {
public:
    // model data
    std::vector<std::shared_ptr<Texture>> textures_loaded;    // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh> meshes;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::string directory;

    Model() = default;
    
    // constructor, expects a filepath to a 3D model.
    explicit Model(const std::string& path) {
        loadModel(path);
    }

    // constructor for geometry data, directly pass in the meshes(vertices, indices, and textures)
    explicit Model(GeometryType geometryType) {
        Mesh mesh(geometryType);
        meshes.push_back(mesh);
    }

    void addTexture2D(const std::string& path, const std::string& variableName) {
        auto texture = loadTexture2D(path);
        texture->variableName = variableName;
        // 检测当前mesh的size是否为1，如果是则添加，不然报错
        if (meshes.size() == 1) {
            meshes[0].textures.push_back(std::shared_ptr<Texture>(texture));
        } else {
            std::cerr << "Error: It is not a common geometry model!" << std::endl;
        }
    }

    void addTextureCube(const std::vector<std::string>& paths, const std::string& variableName) {
        auto texture = loadTextureCube(paths);
        texture->variableName = variableName;
        // 检测当前mesh的size是否为1，如果是则添加，不然报错
        if (meshes.size() == 1) {
            meshes[0].textures.push_back(std::shared_ptr<Texture>(texture));
        } else {
            std::cerr << "Error: It is not a common geometry model!" << std::endl;
        }
    }

    // draws the model, and thus all its meshes
    void draw(Shader* shader = nullptr) {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw(shader);
    }

    Texture* loadTexture2D(const std::string& path);
    Texture* loadTexture2DHDR(const std::string& path);
    Texture* loadTextureCube(const std::vector<std::string>& paths);

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(const std::string& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& variableName);

    std::shared_ptr<Texture> loadTexture2DFromFile(const std::string& filename);
};

#endif // MODEL_H

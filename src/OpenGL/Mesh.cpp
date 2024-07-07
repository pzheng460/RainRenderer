#include "Mesh.h"

void Mesh::draw(Shader &shader, bool setTexture) {
    shader.use();
    if (setTexture) {
        // bind appropriate textures
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        for (unsigned int i = 0; i < textures.size(); ++i) {
            // retrieve texture number (the N in diffuse_textureN) 获取纹理编号（diffuse_textureN中的N）
            std::string number;
            if (textures[i]->variableName == DIFFUSE_NAME)
                number = std::to_string(diffuseNr++);
            else if (textures[i]->variableName == SPECULAR_NAME)
                number = std::to_string(specularNr++); // transfer unsigned int to stream
            else if (textures[i]->variableName == NORMAL_NAME)
                number = std::to_string(normalNr++); // transfer unsigned int to stream
            else if (textures[i]->variableName == HEIGHT_NAME)
                number = std::to_string(heightNr++); // transfer unsigned int to stream

            // now set the sampler to the correct texture unit 现在将采样器设置为正确的纹理单元
            std::string name = textures[i]->variableName;
            shader.setTexture(name + number, textures[i].get());
        }
    }

    geometry->draw();
}
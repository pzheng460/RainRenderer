#include "Mesh.h"

void Mesh::draw(Shader* shader) {
    if (shader != nullptr) {
        shader->use();
        for (const auto& texture : textures) {
            // now set the sampler to the correct texture unit 现在将采样器设置为正确的纹理单元
            std::string name = texture->variableName;
            shader->setTexture(name, texture.get());
        }
    }

    geometry->draw();
}
#include "Mesh.h"

void Mesh::draw(Shader &shader, bool setTexture) {
    shader.use();
    if (setTexture) {
        for (unsigned int i = 0; i < textures.size(); ++i) {
            // now set the sampler to the correct texture unit 现在将采样器设置为正确的纹理单元
            std::string name = textures[i]->variableName;
            shader.setTexture(name, textures[i].get());
        }
    }

    geometry->draw();
}
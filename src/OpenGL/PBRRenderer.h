#ifndef PBR_RENDERER_H
#define PBR_RENDERER_H

#include "Renderer.h"

class PBRRenderer : public Renderer {
public:
    PBRRenderer(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, Scene& scene);
    void initialize() override;
    void renderScene(GUI& gui) override;

private:
    Shader pbrShader;
};

#endif // PBR_RENDERER_H


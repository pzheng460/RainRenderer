#ifndef RENDERER_H
#define RENDERER_H

#include "FrameBuffer.h"
#include "MSAAFrameBuffer.h"
#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>

#include <utility>
#include "Scene.h"
#include "GUI.h"

class Renderer {
public:
    Renderer() = default;
    Renderer(int SCR_WIDTH, int SCR_HEIGHT, std::shared_ptr<GUI>& gui) : SCR_WIDTH(SCR_WIDTH), SCR_HEIGHT(SCR_HEIGHT), gui(gui) {}
    ~Renderer() = default;

    void init();
    void reset();
    void draw(Scene& scene);

    void gaussianBlur();

    void faceCulling() const;
    void MSAA() const;
    void gammaCorrection() const;


    // GUI
    std::shared_ptr<GUI> gui;

    void setFrameBufferSize(pair<int, int> size) {
        SCR_WIDTH = size.first;
        SCR_HEIGHT = size.second;
    }

    FrameBuffer& getMainMSAAFrameBuffer() {
        return mainMSAAFrameBuffer;
    }

    FrameBuffer& getIntermediateFrameBuffer() {
        return intermediateFrameBuffer;
    }

    std::vector<FrameBuffer>& getPingPongFrameBuffers() {
        return pingPongFrameBuffers;
    }
private:
    // Main Shader
    Shader mainShader;

    int SCR_WIDTH, SCR_HEIGHT;

    // Main FrameBuffer
    MSAAFrameBuffer mainMSAAFrameBuffer = MSAAFrameBuffer(2, 1);
    FrameBuffer intermediateFrameBuffer = FrameBuffer(2, 0);

    // Bloom
    Shader shaderBlur = Shader(FileSystem::getPath("src/OpenGL/shaders/blur.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/blur.fs").c_str());
    std::vector<FrameBuffer> pingPongFrameBuffers = std::vector<FrameBuffer>(2);
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;

    // G-Buffer
    Shader gBufferShader;
    FrameBuffer gFrameBuffer = FrameBuffer(3, 1);

    // SSAO
    Shader SSAOShader;
    Shader SSAOBlurShader;
    FrameBuffer SSAOFrameBuffer = FrameBuffer(1, 1);

    // Final
    Shader FinalShader;
};

#endif // _RENDERER_H

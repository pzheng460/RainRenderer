#include "GUI.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <utils/filesystem.h>
#include <stb_image.h>
#include <tinyfiledialogs.h>

GUI::GUI() {
    // 初始化其他参数
}

GUI::~GUI() {
    // 清理 ImGui 资源
}

void GUI::init(GLFWwindow* window) {
    // 初始化 ImGui
    const char* glsl_version = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark(); // Dark style，暗色风格

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // 背景颜色
}

void GUI::render(std::string& modelFilePath, Scene& scene) {
    ImGui_ImplOpenGL3_NewFrame(); // OpenGL 渲染 ImGui
    ImGui_ImplGlfw_NewFrame(); // GLFW 渲染 ImGui
    ImGui::NewFrame(); // ImGui 新帧
    {
        // 创建一个新的窗口并显示信息
        ImGui::Begin("Parameters");

        // 显示帧率
        ImGui::Text("Framerate: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::Separator(); // 分隔线

        // 控制鼠标和键盘开关
        ImGui::Text("Control");
        ImGui::Checkbox("Control (Press space to toggle)", &controlActive);

        ImGui::Separator(); // 分隔线

        // 显示和控制相机参数
        ImGui::Text("Camera Position");
        ImGui::SliderFloat("X", &camera.Position.x, -30.0f, 30.0f);
        ImGui::SliderFloat("Y", &camera.Position.y, -30.0f, 30.0f);
        ImGui::SliderFloat("Z", &camera.Position.z, -30.0f, 30.0f);
        ImGui::Text("Camera Yaw");
        ImGui::SliderFloat("Yaw", &camera.Yaw, -180.0f, 180.0f);
        ImGui::Text("Camera Pitch");
        ImGui::SliderFloat("Pitch", &camera.Pitch, -89.0f, 89.0f);
        ImGui::Text("Camera Roll");
        ImGui::SliderFloat("Roll", &camera.Roll, -180.0f, 180.0f);

        // 更新相机的方向
        camera.updateCameraVectors();

        ImGui::Separator(); // 分隔线

        // 显示和控制光源参数
        for (unsigned int i = 0; i < scene.lights.size(); i++)
        {
            ImGui::Text("Light %d Position", i);
            ImGui::SliderFloat(("X##" + std::to_string(i)).c_str(), &scene.lights[i].position.x, -20.0f, 20.0f);
            ImGui::SliderFloat(("Y##" + std::to_string(i)).c_str(), &scene.lights[i].position.y, -20.0f, 20.0f);
            ImGui::SliderFloat(("Z##" + std::to_string(i)).c_str(), &scene.lights[i].position.z, -20.0f, 20.0f);

            ImGui::Text("Light %d Color", i);
            ImGui::ColorEdit3(("Color##" + std::to_string(i)).c_str(), (float*)&scene.lights[i].color);

            scene.lights[i].updateLightColor();
            scene.lights[i].updateLightSpaceMatrix();
        }

        ImGui::Separator(); // 分隔线

        // Rendering Path 渲染路径
        ImGui::Text("Rendering Path");
        if (ImGui::RadioButton("Forward Rendering", renderingPath == FORWARDRENDERING)) {
            renderingPath = FORWARDRENDERING;
        }
        if (ImGui::RadioButton("Deferred Rendering", renderingPath == DEFERREDRENDERING)) {
            renderingPath = DEFERREDRENDERING;
            mode = BLINNPHONG;
        }

        ImGui::Separator(); // 分隔线

        ImGui::Text("EFFECTS");
        // 开关 SkyBox
        ImGui::Checkbox("Skybox", &skyBoxActive);
        if (!skyBoxActive) { ImGui::BeginDisabled(); }
        {
            ImGui::Text("Skybox Load Mode");
            if (ImGui::RadioButton("Cube Map", skyboxMode == CUBEMAP)) {
                skyboxMode = CUBEMAP;
                Shader skyboxShader(FileSystem::getPath("src/OpenGL/shaders/skybox.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/skybox.fs").c_str());
                vector<std::string> faces =
                        {
                                FileSystem::getPath("resources/textures/skybox/right.jpg"),
                                FileSystem::getPath("resources/textures/skybox/left.jpg"),
                                FileSystem::getPath("resources/textures/skybox/top.jpg"),
                                FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
                                FileSystem::getPath("resources/textures/skybox/front.jpg"),
                                FileSystem::getPath("resources/textures/skybox/back.jpg")
                        };
                Skybox skybox(skyboxShader, faces);
                scene.setSkybox(skybox);
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Sphere Map", skyboxMode == SPHEREMAP)) {
                skyboxMode = SPHEREMAP;
                Shader skyboxShader(FileSystem::getPath("src/OpenGL/shaders/background.vs").c_str(), FileSystem::getPath("src/OpenGL/shaders/background.fs").c_str());
                Skybox skybox(skyboxShader, FileSystem::getPath("resources/textures/hdr/newport_loft.hdr"));
                scene.setSkybox(skybox);
            }
        }
        if (!skyBoxActive) { ImGui::EndDisabled(); }

        // Light 光源
        ImGui::Checkbox("Light", &lightActive);
        ImGui::SameLine();
        // Floor 地板
        ImGui::Checkbox("Floor", &floorActive);

        // face culling 面剔除
        ImGui::Checkbox("Face Culling", &faceCullingActive);
        // Gamma Correction 伽马校正
        ImGui::Checkbox("Gamma Correction", &gammaCorrectionActive);

        if (renderingPath != FORWARDRENDERING) { ImGui::BeginDisabled(); }
        {
            // normal visualization 法线可视化
            ImGui::Checkbox("Normal", &normalVisualizationActive);
            // MSAA 抗锯齿
            ImGui::Checkbox("MSAA", &MSAAActive);
            // HDR 高动态范围
            ImGui::Checkbox("HDR", &HDRActive);
            // Bloom 泛光
            ImGui::Checkbox("Bloom", &bloomActive);
        }
        if (renderingPath != FORWARDRENDERING) { ImGui::EndDisabled(); }

        if (renderingPath != DEFERREDRENDERING) { ImGui::BeginDisabled(); }
        {
            // SSAO 屏幕空间环境光遮蔽
            ImGui::Checkbox("SSAO", &SSAOActive);
        }
        if (renderingPath != DEFERREDRENDERING) { ImGui::EndDisabled(); }

        ImGui::Text("Render Mode");
        if (ImGui::RadioButton("Basic", mode == BASIC)) {
            mode = BASIC;
        }
        if (ImGui::RadioButton("Phong", mode == PHONG)) {
            mode = PHONG;
        }
        if (ImGui::RadioButton("Blinn-Phong", mode == BLINNPHONG)) {
            mode = BLINNPHONG;
        }
        if (ImGui::RadioButton("Depth", mode == DEPTH)) {
            mode = DEPTH;
        }
        if (ImGui::RadioButton("Environment Mapping", mode == ENVIRONMENTMAPPING)) {
            mode = ENVIRONMENTMAPPING;
        }
        if (ImGui::RadioButton("PBR", mode == PBR)) {
            mode = PBR;
        }

        if (mode != BLINNPHONG) { ImGui::BeginDisabled(); }
        {
            ImGui::Text("Shadow");
            ImGui::Checkbox("Shadow", &shadowActive);
        }
        if (mode != BLINNPHONG) { ImGui::EndDisabled(); }

        ImGui::Separator(); // 分隔线

        // 模型导入
        ImGui::Text("Model Import");
        static char filePath[128] = "";
        ImGui::InputText("File Path", filePath, sizeof(filePath));
        if (ImGui::Button("Open File Dialog")) {
            const char *filterPatterns[] = {"*.obj", "*.fbx", "*.dae", "*.pmx"};
            const char *selectedFilePath = tinyfd_openFileDialog(
                    "Select Model File", "", 4, filterPatterns, NULL, 0);
            if (selectedFilePath) {
                strncpy(filePath, selectedFilePath, sizeof(filePath) - 1);
                filePath[sizeof(filePath) - 1] = '\0'; // 确保字符串以 null 结尾
            }
        }
        if (ImGui::Button("Load Model")) {
            if (strlen(filePath) == 0) {
                modelFilePath = FileSystem::getPath(modelFilePath); // 使用默认的模型文件路径
                strcpy(filePath, modelFilePath.c_str());
            } else {
                modelFilePath = std::string(filePath); // 更新模型文件路径
            }

            // load models
            stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis 告诉 stb_image.h 在 y 轴上翻转加载的纹理
            AssimpModel::Model ourModel(modelFilePath);
            auto object = std::make_unique<Object>(ourModel);
            scene.addObject(std::move(object));
        }

        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


#include "GUI.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <utils/filesystem.h>
#include <stb_image.h>
#include <tinyfiledialogs.h>

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

void GUI::render(Scene& scene) {
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
        ImGui::SliderFloat("X", &scene.camera->Position.x, -30.0f, 30.0f);
        ImGui::SliderFloat("Y", &scene.camera->Position.y, -30.0f, 30.0f);
        ImGui::SliderFloat("Z", &scene.camera->Position.z, -30.0f, 30.0f);
        ImGui::Text("Camera Yaw");
        ImGui::SliderFloat("Yaw", &scene.camera->Yaw, -180.0f, 180.0f);
        ImGui::Text("Camera Pitch");
        ImGui::SliderFloat("Pitch", &scene.camera->Pitch, -89.0f, 89.0f);
        ImGui::Text("Camera Roll");
        ImGui::SliderFloat("Roll", &scene.camera->Roll, -180.0f, 180.0f);

        // 更新相机的方向
        scene.camera->updateCameraVectors();

        ImGui::Separator(); // 分隔线

        // 显示和控制光源参数
        for (unsigned int i = 0; i < scene.lights.size(); i++)
        {
            ImGui::Text("Light %d Position", i);
            ImGui::SliderFloat(("X##" + std::to_string(i)).c_str(), &scene.lights[i]->position.x, -20.0f, 20.0f);
            ImGui::SliderFloat(("Y##" + std::to_string(i)).c_str(), &scene.lights[i]->position.y, -20.0f, 20.0f);
            ImGui::SliderFloat(("Z##" + std::to_string(i)).c_str(), &scene.lights[i]->position.z, -20.0f, 20.0f);

            ImGui::Text("Light %d Color", i);
            ImGui::ColorEdit3(("Color##" + std::to_string(i)).c_str(), (float*)&scene.lights[i]->color);

            scene.lights[i]->updateLightColor();
            scene.lights[i]->updateLightSpaceMatrix();
        }

        ImGui::Separator(); // 分隔线

        // Rendering Path 渲染路径
        ImGui::Text("Rendering Path");
        if (ImGui::RadioButton("Forward Rendering", renderingPath == RenderingPath::FORWARD_RENDERING)) {
            renderingPath = RenderingPath::FORWARD_RENDERING;
        }
        if (ImGui::RadioButton("Deferred Rendering", renderingPath == RenderingPath::DEFERRED_RENDERING)) {
            renderingPath = RenderingPath::DEFERRED_RENDERING;
            mode = RenderMode::BLINN_PHONG;
        }

        ImGui::Separator(); // 分隔线

        ImGui::Text("EFFECTS");
        // 开关 SkyBox
        ImGui::Checkbox("Skybox", &skyBoxActive);
        if (!skyBoxActive) { ImGui::BeginDisabled(); }
        {
            ImGui::Text("Skybox Load Mode");
            if (ImGui::RadioButton("Cube Map", skyboxMode == SkyboxLoadMode::CUBE_MAP)) {
                skyboxMode = SkyboxLoadMode::CUBE_MAP;
                std::vector<std::string> paths =
                        {
                                FileSystem::getPath("resources/textures/skybox/right.jpg"),
                                FileSystem::getPath("resources/textures/skybox/left.jpg"),
                                FileSystem::getPath("resources/textures/skybox/top.jpg"),
                                FileSystem::getPath("resources/textures/skybox/bottom.jpg"),
                                FileSystem::getPath("resources/textures/skybox/front.jpg"),
                                FileSystem::getPath("resources/textures/skybox/back.jpg")
                        };
                Skybox skybox(paths);
                scene.setSkybox(skybox);
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Sphere Map", skyboxMode == SkyboxLoadMode::SPHERE_MAP)) {
                skyboxMode = SkyboxLoadMode::SPHERE_MAP;
                Skybox skybox(FileSystem::getPath("resources/textures/hdr/newport_loft.hdr"));
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
        // HDR 高动态范围
        ImGui::Checkbox("HDR", &HDRActive);
        // Bloom 泛光
        ImGui::Checkbox("Bloom", &bloomActive);

        if (renderingPath != RenderingPath::FORWARD_RENDERING) { ImGui::BeginDisabled(); }
        {
            // normal visualization 法线可视化
            ImGui::Checkbox("Normal", &normalVisualizationActive);
            // MSAA 抗锯齿
            ImGui::Checkbox("MSAA", &MSAAActive);
        }
        if (renderingPath != RenderingPath::FORWARD_RENDERING) { ImGui::EndDisabled(); }

        if (renderingPath != RenderingPath::DEFERRED_RENDERING) { ImGui::BeginDisabled(); }
        {
            // SSAO 屏幕空间环境光遮蔽
            ImGui::Checkbox("SSAO", &SSAOActive);
        }
        if (renderingPath != RenderingPath::DEFERRED_RENDERING) { ImGui::EndDisabled(); }

        ImGui::Text("Render Mode");
        if (ImGui::RadioButton("Basic", mode == RenderMode::BASIC)) {
            mode = RenderMode::BASIC;
        }
        if (ImGui::RadioButton("Phong", mode == RenderMode::PHONG)) {
            mode = RenderMode::PHONG;
        }
        if (ImGui::RadioButton("Blinn-Phong", mode == RenderMode::BLINN_PHONG)) {
            mode = RenderMode::BLINN_PHONG;
        }
        if (ImGui::RadioButton("Depth", mode == RenderMode::DEPTH)) {
            mode = RenderMode::DEPTH;
        }
        if (ImGui::RadioButton("Environment Mapping", mode == RenderMode::ENVIRONMENT_MAPPING)) {
            mode = RenderMode::ENVIRONMENT_MAPPING;
        }
        if (ImGui::RadioButton("PBR", mode == RenderMode::PBR)) {
            mode = RenderMode::PBR;
        }

        if (mode != RenderMode::BLINN_PHONG) { ImGui::BeginDisabled(); }
        {
            ImGui::Text("Shadow");
            ImGui::Checkbox("Shadow", &shadowActive);
        }
        if (mode != RenderMode::BLINN_PHONG) { ImGui::EndDisabled(); }

        ImGui::End();
    }

    {
        ImGui::Begin("Object");

        // 模型导入
        ImGui::Text("Model Import");
        static char filePath[128] = "";
        ImGui::InputText("File Path", filePath, sizeof(filePath));
        if (ImGui::Button("Open File Dialog")) {
            const char *filterPatterns[] = {"*.obj", "*.fbx", "*.dae", "*.pmx", "*.gltf"};
            const char *selectedFilePath = tinyfd_openFileDialog("Select Model File", "", 5, filterPatterns, NULL, 0);
            if (selectedFilePath) {
                strncpy(filePath, selectedFilePath, sizeof(filePath) - 1);
                filePath[sizeof(filePath) - 1] = '\0'; // 确保字符串以 null 结尾
            }
        }
        if (ImGui::Button("Load Model")) {
            std::string modelFilePath;
            if (strlen(filePath) == 0) {
                modelFilePath = FileSystem::getPath(std::string(DEFAULT_FILE_PATH)); // 使用默认的模型文件路径
                strcpy(filePath, modelFilePath.c_str());
            } else {
                modelFilePath = std::string(filePath); // 更新模型文件路径
            }

            // load models
            stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis 告诉 stb_image.h 在 y 轴上翻转加载的纹理
            Model ourModel(modelFilePath);
            auto object = Object(ourModel);
            scene.addObject(object);
        }

        ImGui::Separator(); // 分隔线

        ImGui::Text("Object Information");

        for (int i = 0; i < scene.objects.size(); ++i) {
            auto object = scene.objects[i].get();
            ImGui::Text("Object %d", i);

            // Position controls
            ImGui::Text("Position");
            ImGui::SliderFloat(("PosX##" + std::to_string(i)).c_str(), &object->position.x, -100.0f, 100.0f);
            ImGui::SliderFloat(("PosY##" + std::to_string(i)).c_str(), &object->position.y, -100.0f, 100.0f);
            ImGui::SliderFloat(("PosZ##" + std::to_string(i)).c_str(), &object->position.z, -100.0f, 100.0f);

            // Rotation controls
            ImGui::Text("Rotation");
            ImGui::SliderFloat(("RotX##" + std::to_string(i)).c_str(), &object->rotation.x, -180.0f, 180.0f);
            ImGui::SliderFloat(("RotY##" + std::to_string(i)).c_str(), &object->rotation.y, -180.0f, 180.0f);
            ImGui::SliderFloat(("RotZ##" + std::to_string(i)).c_str(), &object->rotation.z, -180.0f, 180.0f);

            // Scale controls
            ImGui::Text("Scale");
            ImGui::SliderFloat(("Scale##" + std::to_string(i)).c_str(), &object->scale, 0.01f, 10.0f);

            // delete object 删除物体
            if (ImGui::Button(("Delete##" + std::to_string(i)).c_str())) {
                scene.objects.erase(scene.objects.begin() + i);
            }
        }

        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


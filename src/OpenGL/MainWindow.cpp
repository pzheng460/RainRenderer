#include "MainWindow.h"

bool MainWindow::firstMouse = true;
float MainWindow::lastX = (float) SCR_WIDTH / 2.0;
float MainWindow::lastY = (float) SCR_HEIGHT / 2.0;
std::shared_ptr<GUI> MainWindow::gui = nullptr;
std::shared_ptr<Camera> MainWindow::camera = nullptr;

MainWindow::MainWindow(unsigned int width, unsigned int height, const std::string& title)
        : width(width)
        , height(height)
        , title(title) {}

MainWindow::~MainWindow() {
    // glfw: terminate, clearing all previously allocated GLFW resources. 清理所有之前分配的GLFW资源
    // ------------------------------------------------------------------
    glfwTerminate();
}

bool MainWindow::init() {
    // glfw: initialize and configure 初始化和配置
    // ------------------------------
    // Initialize GLFW 初始化GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Configure GLFW 设置GLFW

    // 设置指定版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // MSAA 多重采样抗锯齿
    glfwWindowHint(GLFW_SAMPLES, 4);

    // 使用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__ // Mac OS X 需要指定兼容
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation 创建窗口
    // --------------------
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    // make window the current context 将窗口设置为OpenGL当前的上下文
    glfwMakeContextCurrent(window);

    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Set callbacks 设置回调
    // --------------------
    // 设置窗口尺寸改变的回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 设置鼠标移动回调
    glfwSetCursorPosCallback(window, mouse_callback);
    // 设置鼠标滚轮回调
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers 加载所有OpenGL函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // configure global opengl state 设置全局opengl状态
    // -----------------------------
    // depth test 深度测试
    glEnable(GL_DEPTH_TEST);
    // set depth function to less than AND equal for skybox depth trick. 设置深度函数为小于等于，用于天空盒深度技巧
    glDepthFunc(GL_LESS); // default value 默认值，这一行可有可无，使用less可以确保early-z测试

    // stencil testing 启动模板测试
    glEnable(GL_STENCIL_TEST);
    // stencil function 模版函数
    // 不等于1的片段通过，这里所有的模版值默认都是0，所以所有片段都会通过
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // stencil operation 模板操作
    // 模版测试失败时，保持模版缓冲区的内容；模版测试通过但深度测试失败，保持模版缓冲区的内容；模版测试和深度测试都通过时，也将模版缓冲区的值设置为ref的1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // 立方体贴图无缝采样

    return true;
}

// check if the window should close 检查窗口是否应该关闭
bool MainWindow::shouldClose() const {
    return glfwWindowShouldClose(window);
}

// per-frame logic before rendering 每一帧渲染前的逻辑
void MainWindow::preRender() {
    // per-frame time logic 每一帧的时间逻辑
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // 处理用户输入
    processInput(window);
}

std::pair<int, int> MainWindow::reset() {
    // 设置清屏颜色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // 清空颜色缓冲、深度缓冲、模板缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // update stencil buffer 保证默认情况下，不会更新模板缓冲区
    glStencilMask(0x00);

    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    width = scrWidth;
    height = scrHeight;
    // set viewport to the size of the framebuffer 设置视口大小为帧缓冲区的大小
    // 参数：左下角的横坐标x，左下角的纵坐标y，宽度，高度
    glViewport(0, 0, scrWidth, scrHeight);
    return {scrWidth, scrHeight};
}

// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
void MainWindow::swapBuffersAndPollEvents() const {
    // 交换颜色缓冲
    glfwSwapBuffers(window);
    // 检查触发事件（如键盘输入、鼠标移动、窗口更新等）
    glfwPollEvents();
}

// whenever the window size changed (by OS or user resize) this callback function executes
void MainWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// whenever the mouse moves, this callback is called
void MainWindow::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!gui->controlActive)
    {
        return;
    }
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

// whenever the mouse scroll wheel scrolls, this callback is called
void MainWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!gui->controlActive) {
        return;
    }
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void MainWindow::processInput(GLFWwindow *window)
{
    // 检测用户是否点击了esc，如果是，则将shouldClose置为true
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // 在函数或方法的开始位置初始化或更新光标状态
    if (gui->controlActive) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    static bool spaceKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!spaceKeyPressed) {
            gui->ToggleControl();  // 切换控制状态
            if (gui->controlActive) {
                // 禁用光标
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else {
                // 启用光标
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            spaceKeyPressed = true;
        }
    } else {
        spaceKeyPressed = false;
    }

    if (gui->controlActive) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    }
}



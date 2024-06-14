#include "Window.h"

std::shared_ptr<GUI> Window::gui = nullptr;

Window::Window(unsigned int width, unsigned int height, const std::string& title)
        : width(width), height(height), title(title), window(nullptr) {}

Window::~Window() {
    // glfw: terminate, clearing all previously allocated GLFW resources. 清理所有之前分配的GLFW资源
    // ------------------------------------------------------------------
    glfwTerminate();
}

bool Window::init() {
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
    glDepthFunc(GL_LEQUAL);

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

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions 然后在渲染之前，将视口配置为原始帧缓冲区的屏幕尺寸
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    // set viewport to the size of the framebuffer 设置视口大小为帧缓冲区的大小
    // 参数：左下角的横坐标x，左下角的纵坐标y，宽度，高度
    glViewport(0, 0, scrWidth, scrHeight);

    return true;
}

// check if the window should close 检查窗口是否应该关闭
bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

// per-frame logic before rendering 每一帧渲染前的逻辑
void Window::preRender() {
    // per-frame time logic 每一帧的时间逻辑
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // 处理用户输入
    processInput(window);

    // 设置清屏颜色
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // 清空颜色缓冲、深度缓冲、模板缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // update stencil buffer 保证默认情况下，不会更新模板缓冲区
    glStencilMask(0x00);
}

// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
void Window::swapBuffersAndPollEvents() const {
    // 交换颜色缓冲
    glfwSwapBuffers(window);
    // 检查触发事件（如键盘输入、鼠标移动、窗口更新等）
    glfwPollEvents();
}

void Window::preOutlineSetting() const {
    // update stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
}

void Window::outlineSetting() const {
    // only the stencil value who is not equal to 1 can pass 只有模板缓冲区的值不等于1的片段才会通过，可以绘制
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    // 禁止写入模板缓冲区，防止preOutlineSetting中已经写入的模板缓冲区被修改，因为GL_REPLACE会将模板缓冲区的值设置为ref的1
    glStencilMask(0x00);
    // 禁用深度测试，防止有物体遮挡轮廓
    glDisable(GL_DEPTH_TEST);
}

void Window::afterOutlineSetting() const {
    // reset stencil test
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST); // 重新启用深度测试
}

// whenever the window size changed (by OS or user resize) this callback function executes
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// whenever the mouse moves, this callback is called
void Window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!gui->IsControlActive())
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

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// whenever the mouse scroll wheel scrolls, this callback is called
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (!gui->IsControlActive())
    {
        return;
    }
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void Window::processInput(GLFWwindow *window)
{
    // 检测用户是否点击了esc，如果是，则将shouldClose置为true
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // 在函数或方法的开始位置初始化或更新光标状态
    if (gui->IsControlActive()) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    static bool spaceKeyPressed = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!spaceKeyPressed) {
            gui->ToggleControl();  // 切换控制状态
            if (gui->IsControlActive()) {
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

    if (gui->IsControlActive())
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}



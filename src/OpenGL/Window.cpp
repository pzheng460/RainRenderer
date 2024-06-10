#include "Window.h"
#include <learnopengl/callback.h>

Window::Window(unsigned int width, unsigned int height, const std::string& title)
        : width(width), height(height), title(title), window(nullptr) {}

Window::~Window() {
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

bool Window::init() {
    // glfw: initialize and configure 初始化和配置
    // ------------------------------
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4); // 多重采样抗锯齿
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__ // MacOS系统需要设置这个才能正常使用
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation 创建窗口
    // --------------------
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Set callbacks
    callback(window);

    // glad: load all OpenGL function pointers 加载所有OpenGL函数指针
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // configure global opengl state 设置全局opengl状态
    // -----------------------------
    glEnable(GL_DEPTH_TEST); // 深度测试
    // set depth function to less than AND equal for skybox depth trick.
    glDepthFunc(GL_LEQUAL); // 深度测试函数
    // enable seamless cubemap sampling for lower mip levels in the pre-filter map.
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // 立方体贴图无缝采样

    // then before rendering, configure the viewport to the original framebuffer's screen dimensions
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    return true;
}

// glfw: check if the window should close
// ---------------------------------------
bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
// -------------------------------------------------------------------------------
void Window::swapBuffersAndPollEvents() const {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

GLFWwindow* Window::getGLFWwindow() const {
    return window;
}

void Window::callback(GLFWwindow* window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 窗口大小改变回调
    glfwSetCursorPosCallback(window, mouse_callback); // 鼠标移动回调
    glfwSetScrollCallback(window, scroll_callback); // 鼠标滚轮回调
}


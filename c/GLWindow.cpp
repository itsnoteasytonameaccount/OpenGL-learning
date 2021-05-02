#include "GLWindow.h"
static int WIDTH, HEIGHT;

static void _mouse_callback(GLFWwindow *window, double xpos, double ypos);
static void _scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
static void _setViewport(GLFWwindow *window, int width, int height);
int GLWindow::initWindow(int width, int height, const char *title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "create window failed" << std::endl;
        glfwTerminate();
        return CREATE_WINDOW_FAILED;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, setViewport);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "glad load failed" << std::endl;
        glfwTerminate();
        return GLAD_LOAD_FAILED;
    }
    setViewport(window, width, height);
    return 0;
}
void GLWindow::getKeyInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}
void GLWindow::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        delta = currentTime - lastTime;
        lastTime = currentTime;
        getKeyInput();
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

GLWindow::GLWindow(/* args */)
{
    this->mouse_callback = _mouse_callback;
    this->scroll_callback = _scroll_callback;
    this->setViewport = _setViewport;
}

GLWindow::~GLWindow()
{
    glfwTerminate();
}
GLWindow::GLWindow(int width, int height, const char *title)
{
    this->mouse_callback = _mouse_callback;
    this->scroll_callback = _scroll_callback;
    this->setViewport = _setViewport;
    lastTime = (float)glfwGetTime();
    initWindow(width, height, title);
}
float GLWindow::getWidth()
{
    return WIDTH;
}
float GLWindow::getHeight()
{
    return HEIGHT;
}

void _mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
}
void _scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}
void _setViewport(GLFWwindow *window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}
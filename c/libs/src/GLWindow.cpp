#include <GLWindow.h>
static int WIDTH = 800, HEIGHT = 600;

static void _setViewport(GLFWwindow *window, int width, int height);
int GLWindow::initWindow(int width, int height, const char *title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        // std::cout << "create window failed" << std::endl;
        glfwTerminate();
        return CREATE_WINDOW_FAILED;
    }
    glfwMakeContextCurrent(window);
    setViewport &&glfwSetFramebufferSizeCallback(window, setViewport);
    mouseCallback &&glfwSetCursorPosCallback(window, mouseCallback);
    scrollCallback &&glfwSetScrollCallback(window, scrollCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // std::cout << "glad load failed" << std::endl;
        glfwTerminate();
        return GLAD_LOAD_FAILED;
    }
    setViewport(window, width, height);
    lastTime = (float)glfwGetTime();
    return 0;
}

int GLWindow::initWindow(const char *title)
{
    return initWindow(WIDTH, HEIGHT, title);
}

void GLWindow::getKeyInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        std::cout << "window:" << window << std::endl;
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
    this->mouseCallback = NULL;
    this->scrollCallback = NULL;
    this->setViewport = _setViewport;
}

GLWindow::~GLWindow()
{
    glfwTerminate();
}
GLWindow::GLWindow(int width, int height, SetViewport setViewport)
{
    this->setViewport = setViewport;
    WIDTH = width;
    HEIGHT = height;
    mouseCallback = NULL;
    scrollCallback = NULL;
}
float GLWindow::getWidth()
{
    return WIDTH;
}
float GLWindow::getHeight()
{
    return HEIGHT;
}

bool GLWindow::getKeyPress(int key, int status)
{
    return glfwGetKey(window, key) == status;
}

void _setViewport(GLFWwindow *window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

float GLWindow::getTime()
{
    return (float)glfwGetTime();
}

void GLWindow::setShouldClose(bool flag)
{
    glfwSetWindowShouldClose(window, flag);
}
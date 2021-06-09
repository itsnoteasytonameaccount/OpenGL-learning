#include <GLWindow.h>
static int WIDTH = 800, HEIGHT = 600;
static Camera *camera = NULL;

int GLWindow::initWindow(const char *title, int width, int height, SetViewport setViewport, MouseCallback mouseCallback, ScrollCallback scrollCallback)
{
    WIDTH = width;
    HEIGHT = height;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(WIDTH, HEIGHT, title, NULL, NULL);
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
    setViewport(window, WIDTH, HEIGHT);
    lastTime = (float)glfwGetTime();
    return 0;
}

void GLWindow::getKeyInput()
{
    direction = 0x00;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        setShouldClose(true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        direction |= _CAMERA_UP;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        direction |= _CAMERA_DOWN;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        direction |= _CAMERA_RIGHT;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        direction |= _CAMERA_LEFT;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        direction |= _CAMERA_FORWARD;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        direction |= _CAMERA_BACKWARD;
    }
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

GLWindow::GLWindow(Camera *cam)
{
    glfwInit();
    camera = cam;
}

GLWindow::~GLWindow()
{
    glfwTerminate();
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

void _mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    if (camera != NULL)
        camera->ProcessMouseMovement(xpos, ypos);
}

void _scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}

float GLWindow::getTime()
{
    return (float)glfwGetTime();
}

void GLWindow::setShouldClose(bool flag)
{
    glfwSetWindowShouldClose(window, flag);
}
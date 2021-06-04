#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Camera.hpp>

#ifndef __GLWINDOW_CLASS__
#define __GLWINDOW_CLASS__
typedef void (*SetViewport)(GLFWwindow *, int, int);
typedef void (*MouseCallback)(GLFWwindow *, double, double);
typedef void (*ScrollCallback)(GLFWwindow *, double, double);

void _setViewport(GLFWwindow *window, int width, int height);
void _mouseCallback(GLFWwindow *window, double xpos, double ypos);

class GLWindow
{
private:
    /* data */

protected:
    GLFWwindow *window;
    float lastTime, delta;
    short direction;

public:
    const static int CREATE_WINDOW_FAILED = 1;
    const static int GLAD_LOAD_FAILED = 2;

    GLWindow(Camera * = NULL);
    ~GLWindow();

    // 纯虚函数
    virtual void draw() = 0;

    virtual void getKeyInput();
    virtual void mainLoop();
    virtual float getWidth();
    virtual float getHeight();

    int initWindow(const char *title, int width = 800, int height = 600, SetViewport = _setViewport, MouseCallback = _mouseCallback, ScrollCallback = NULL);

    float getTime();
    bool getKeyPress(int, int = GLFW_PRESS);
    void setShouldClose(bool flag);
};
#endif
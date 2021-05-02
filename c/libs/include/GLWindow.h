#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#ifndef __GLWINDOW_CLASS__
#define __GLWINDOW_CLASS__
typedef void (*SetViewport)(GLFWwindow *, int, int);
typedef void (*MouseCallback)(GLFWwindow *, double, double);
typedef void (*ScrollCallback)(GLFWwindow *, double, double);

class GLWindow
{
private:
    /* data */

protected:
    GLFWwindow *window;
    float lastTime, delta;
    MouseCallback mouseCallback;
    ScrollCallback scrollCallback;
    SetViewport setViewport;

public:
    const static int CREATE_WINDOW_FAILED = 1;
    const static int GLAD_LOAD_FAILED = 2;
    GLWindow(/* args */);
    GLWindow(int width, int height, SetViewport callback);
    ~GLWindow();
    // 纯虚函数
    virtual void draw() = 0;
    virtual void getKeyInput();
    virtual void mainLoop();
    virtual float getWidth();
    virtual float getHeight();
    int initWindow(int width, int height, const char *title);
    int initWindow(const char *title);
    bool getKeyPress(int, int = GLFW_PRESS);
    float getTime();
    void setShouldClose(bool flag);
};
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#ifndef __GLWINDOW_CLASS__
#define __GLWINDOW_CLASS__
class GLWindow
{
private:
    /* data */
protected:
    GLFWwindow *window;
    float lastTime, delta;
    void (*mouse_callback)(GLFWwindow *window, double xpos, double ypos);
    void (*scroll_callback)(GLFWwindow *window, double xoffset, double yoffset);
    void (*setViewport)(GLFWwindow *, int, int);

public:
    const static int CREATE_WINDOW_FAILED = 1;
    const static int GLAD_LOAD_FAILED = 2;
    GLWindow(/* args */);
    GLWindow(int width, int height, const char *title);
    ~GLWindow();
    virtual void draw() = 0;
    virtual void getKeyInput();
    int initWindow(int width, int height, const char *title);
    void mainLoop();
    float getWidth();
    float getHeight();
};
#endif
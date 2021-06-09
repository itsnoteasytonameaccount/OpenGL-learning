#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Object.h"

#ifndef CUBE_H
#define CUBE_H
class Cube
{
private:
    unsigned int vbo, ebo, vao, id;
    /* data */
protected:
    static float vertices[];

public:
    Cube(unsigned int id);
    void createVAO();
    void draw();
    void bind();
    ~Cube();
};

#endif
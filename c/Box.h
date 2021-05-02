#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLWindow.h"
#include "Shader.h"
#include "Texture.h"
#include "examples/Camera.hpp"
// #include "Camera.h"

static glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(2.0f, 5.0f, -15.0f),
                                    glm::vec3(-1.5f, -2.2f, -2.5f),
                                    glm::vec3(-3.8f, -2.0f, -12.3f),
                                    glm::vec3(2.4f, -0.4f, -3.5f),
                                    glm::vec3(-1.7f, 3.0f, -7.5f),
                                    glm::vec3(1.3f, -2.0f, -2.5f),
                                    glm::vec3(1.5f, 2.0f, -2.5f),
                                    glm::vec3(1.5f, 0.2f, -1.5f),
                                    glm::vec3(-1.3f, 1.0f, -1.5f)};
class Box : public GLWindow
{
private:
    Shader *shader;
    unsigned int VAO, VBO;
    float lastTime;
    /* data */
public:
    Box(/* args */);
    ~Box();
    void loadShader();
    void createVAO(unsigned int texture1, unsigned int texture2);
    void draw();
    void initShader();
    void getKeyInput();
    Shader *getShader();
};

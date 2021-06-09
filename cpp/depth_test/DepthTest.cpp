#include "DepthTest.h"

static Camera camera;
static glm::mat4 projection;
static int WIDTH = 800, HEIGHT = 600;

static const char *const vertex_shader_path = "dist/shader/depth_test/depth_test.vs";
static const char *const fragment_shader_path = "dist/shader/depth_test/depth_test.fs";

static float planeVertices[] = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
    5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
    -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

    5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
    5.0f, -0.5f, -5.0f, 2.0f, 2.0f};

void _setViewport(GLFWwindow *window, int width, int height)
{
    float aspect;
    WIDTH = width;
    HEIGHT = height;
    aspect = height == 0 ? 0.0f : (float)width / height;
    projection = glm::perspective(glm::radians(camera.getZoom()), (float)aspect, 0.1f, 100.0f);
    glViewport(0, 0, width, height);
}

void _mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    camera.ProcessMouseMovement(xpos, ypos);
}

DepthTest::DepthTest(/* args */) : box(1)
{
    initWindow("深度测试场景", WIDTH, HEIGHT, _setViewport, _mouseCallback);

    shader.readFile(vertex_shader_path, fragment_shader_path);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    // glClearStencil(0x00);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    Texture::LoadTexture("dist/assets/depth_test/marble.jpg", &texture1, 1, 0);
    Texture::LoadTexture("dist/assets/depth_test/metal.png", &texture2, 1, 1);

    box.createVAO();
    createFloorVAO();
}

void DepthTest::draw()
{
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 model(1.0f);
    glm::mat4 view = camera.getViewMatrix();

    shader.useProgram();
    shader.setUniform1i("is_texture", 1);
    // glDepthFunc(GL_ALWAYS);
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0x00);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    shader.setUniform1i("tex", 1);
    shader.setUniformMatrix4("model", glm::mat4(1.0f));
    shader.setUniformMatrix4("view", view);
    shader.setUniformMatrix4("projection", projection);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    shader.setUniform1i("tex", 0);

    glStencilMask(0xff);
    // glDepthFunc(GL_LESS);

    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
    shader.setUniformMatrix4("model", model);
    box.bind();
    box.draw();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
    shader.setUniformMatrix4("model", model);
    box.draw();

    shader.setUniform1i("is_texture", 0);
    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glStencilMask(0x00);

    model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
    shader.setUniformMatrix4("model", model);
    box.draw();
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    shader.setUniformMatrix4("model", model);
    box.draw();
    glStencilMask(0xff);
}

void DepthTest::createFloorVAO()
{
    glGenVertexArrays(2, &vao);
    glGenBuffers(2, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(int)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

DepthTest::~DepthTest()
{
    glDeleteVertexArrays(2, &vao);
    glDeleteBuffers(2, &vbo);
}

void DepthTest::getKeyInput()
{

    int direction = 0x00;
    if (getKeyPress(GLFW_KEY_ESCAPE))
    {
        setShouldClose(true);
    }
    if (getKeyPress(GLFW_KEY_UP, GLFW_PRESS))
    {
        direction |= _CAMERA_UP;
    }
    if (getKeyPress(GLFW_KEY_DOWN))
    {
        direction |= _CAMERA_DOWN;
    }
    if (getKeyPress(GLFW_KEY_RIGHT))
    {
        direction |= _CAMERA_RIGHT;
    }
    if (getKeyPress(GLFW_KEY_LEFT))
    {
        direction |= _CAMERA_LEFT;
    }
    if (getKeyPress(GLFW_KEY_W))
    {
        direction |= _CAMERA_FORWARD;
    }
    if (getKeyPress(GLFW_KEY_S))
    {
        direction |= _CAMERA_BACKWARD;
    }
    if (direction)
    {
        camera.ProcessKeyboardInput(direction, delta);
    }
}
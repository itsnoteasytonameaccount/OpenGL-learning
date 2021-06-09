#include "Blending.h"

static Camera camera;
static glm::mat4 projection;
static int WIDTH = 800, HEIGHT = 600;

static const char *const vertex_shader_path = "dist/shader/depth_test/depth_test.vs";
static const char *const fragment_shader_path = "dist/shader/blending/blending.fs";

using std::map;
using std::vector;

vector<glm::vec3> vegetation;

static float planeVertices[] = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
    -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
    5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

    5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
    5.0f, -0.5f, -5.0f, 2.0f, 2.0f};

static float square[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 0.0f};

static unsigned int indices[] = {
    0, 1, 2, 0, 2, 3};

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

Blending::Blending(/* args */) : box(1)
{
    initWindow("深度测试场景", WIDTH, HEIGHT, _setViewport, _mouseCallback);

    shader.readFile(vertex_shader_path, fragment_shader_path);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);

    Texture::LoadTexture("dist/assets/depth_test/marble.jpg", &texture1, 1, 0);
    Texture::LoadTexture("dist/assets/depth_test/metal.png", &texture2, 1, 1);
    Texture::LoadTexture("dist/assets/blending/grass.png", &texture3, 1, 2, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    Texture::LoadTexture("dist/assets/blending/blending_transparent_window.png", &texture4, 1, 3, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    box.createVAO();
    createFloorVAO();
    createSquareVAO();

    vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    c.initImGui(window);
}

void Blending::draw()
{
    c.checkFlags();
    c.checkCullOptions();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model(1.0f);
    glm::mat4 view = camera.getViewMatrix();

    shader.useProgram();
    shader.setUniform1i("is_texture", 1);
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

    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
    shader.setUniformMatrix4("model", model);
    box.bind();
    box.draw();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
    shader.setUniformMatrix4("model", model);
    box.draw();

    glBindVertexArray(vao1);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture4);
    shader.setUniform1i("tex", 3);

    // glDepthMask(GL_FALSE);
    map<float, glm::vec3> sorted;
    float distance;
    for (int i = 0; i < vegetation.size(); i++)
    {
        distance = glm::length(camera.getPosition() - vegetation[i]);
        sorted[distance] = vegetation[i];
        // model = glm::translate(glm::mat4(1.0f), vegetation[i - 1]);
        // shader.setUniformMatrix4("model", model);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    for (map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
    {
        model = glm::translate(glm::mat4(1.0f), it->second);
        shader.setUniformMatrix4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    // glDepthMask(GL_TRUE);

    c.functionSwitch();
}

void Blending::createFloorVAO()
{
    glGenVertexArrays(2, &vao);
    glGenBuffers(2, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Blending::createSquareVAO()
{
    glGenVertexArrays(3, &vao1);
    glGenBuffers(3, &vbo1);
    glGenBuffers(3, &ebo1);
    glBindVertexArray(vao1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Blending::~Blending()
{
    glDeleteVertexArrays(2, &vao);
    glDeleteVertexArrays(3, &vao1);
    glDeleteBuffers(2, &vbo);
    glDeleteBuffers(3, &vbo1);
    glDeleteBuffers(3, &ebo1);
}

void Blending::getKeyInput()
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
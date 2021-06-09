#include "Framebuffers.h"

static Camera camera;
static glm::mat4 projection;
static int WIDTH = 800, HEIGHT = 600;

static const char *const vertex_shader_path = "dist/shader/depth_test/depth_test.vs";
static const char *const fragment_shader_path = "dist/shader/depth_test/depth_test.fs";
static const char *const vertex_shader_path1 = "dist/framebuffers/shader/framebuffers.vs";
static const char *const fragment_shader_path1 = "dist/framebuffers/shader/framebuffers.fs";

static float planeVertices[] = {
    // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
    5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
    -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

    5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
    5.0f, -0.5f, -5.0f, 2.0f, 2.0f};

static float square[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

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

Framebuffers::Framebuffers(/* args */) : box(1)
{
    initWindow("深度测试场景", WIDTH, HEIGHT, _setViewport, _mouseCallback);

    shader.readFile(vertex_shader_path, fragment_shader_path);
    framebuffers_shader.readFile(vertex_shader_path1, fragment_shader_path1);

    c.initImGui(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    // glClearStencil(0x00);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Texture::LoadTexture("dist/framebuffers/assets/container.jpg", &texture1, 1, 0);
    Texture::LoadTexture("dist/assets/depth_test/metal.png", &texture2, 1, 1);

    box.createVAO();
    createFloorVAO();
    createFramebuffer();
    createSquareVAO();
}

void Framebuffers::createSquareVAO()
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
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Framebuffers::draw()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model(1.0f);
    glm::mat4 view = camera.getViewMatrix();

    shader.useProgram();
    shader.setUniform1i("is_texture", 1);
    // glDepthFunc(GL_ALWAYS);
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
    shader.setUniformMatrix4("model", model);
    box.bind();
    box.draw();

    model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
    shader.setUniformMatrix4("model", model);
    box.draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    framebuffers_shader.useProgram();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture3);
    framebuffers_shader.setUniform1i("tex", 0);
    framebuffers_shader.setUniform1i("effect", c.effect);
    if (c.effect == -2)
    {
        glDrawElements(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    c.effectSwitch();
}

void Framebuffers::createFloorVAO()
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

Framebuffers::~Framebuffers()
{
    glDeleteVertexArrays(2, &vao);
    glDeleteVertexArrays(3, &vao1);
    glDeleteBuffers(2, &vbo);
    glDeleteBuffers(3, &vbo1);
    glDeleteBuffers(3, &ebo1);
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    glDeleteTextures(1, &texture3);
}

void Framebuffers::getKeyInput()
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

void Framebuffers::createFramebuffer()
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    texture3 = Texture::CreateTexture(WIDTH, HEIGHT, 1);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture3, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "framebuffer create error" << std::endl;
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
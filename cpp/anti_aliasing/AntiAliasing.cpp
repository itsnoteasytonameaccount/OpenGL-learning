#include "AntiAliasing.h"

static const char *const vertex_shader_path = "dist/anti_aliasing/shader/anti_aliasing.vs";
static const char *const fragment_shader_path = "dist/anti_aliasing/shader/anti_aliasing.fs";
static int WIDTH = 800, HEIGHT = 600;
static glm::mat4 projection;
static Camera camera;
static float degree = 0.0f;
static unsigned int ubo;
static bool after_create = false;

static float square[] = {
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f};

static char *labels[] = {
    "anti alias off",
    "anti alias on",
    "anti alias & inversion",
    "anti alias & gray scale",
    "anti alias & blur",
    "anti alias & sharpen",
    "anti alias & edge detection",
    "custom anti alias"};

void updateUniformBuffer()
{
    if (after_create)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

void _setViewport(GLFWwindow *window, int width, int height)
{
    float aspect;
    WIDTH = width;
    HEIGHT = height;
    aspect = height == 0 ? 0.0f : (float)width / height;
    projection = glm::perspective(glm::radians(camera.getZoom()), (float)aspect, 0.1f, 100.0f);
    glViewport(0, 0, width, height);
    updateUniformBuffer();
}

void _mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    camera.ProcessMouseMovement(xpos, ypos);
}

AntiAliasing::AntiAliasing() : box(1), c(labels, 8, "effect")
{
    initWindow("抗锯齿", WIDTH, HEIGHT, _setViewport, _mouseCallback);

    shader.readFile(vertex_shader_path, fragment_shader_path);
    square_shader.readFile("dist/anti_aliasing/shader/anti_aliasing_square.vs", "dist/anti_aliasing/shader/anti_aliasing_square.fs");

    createUniformBuffer();
    shader.setUniformBlockBinding("matrices", 0);
    // square_shader.setUniformBlockBinding("matrices", 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    box.createVAO();
    createVAO();
    createMultisampleFramebuffer();
    createFramebuffer();

    Texture::LoadTexture("dist/framebuffers/assets/container.jpg", &texture2, 1, 0);

    c.initImGui(window);
}

AntiAliasing::~AntiAliasing()
{
    glDeleteFramebuffers(1, &fbo);
    glDeleteFramebuffers(1, &fbo1);
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteRenderbuffers(1, &rbo1);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ubo);
}

void AntiAliasing::draw()
{
    EffectRadioButton *erb = (EffectRadioButton *)c.getDrawer();
    if (erb->mode == 0)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo1);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model(1.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    shader.useProgram();
    shader.setUniformMatrix4("model", model);
    box.bind();
    box.draw();

    if (erb->mode != 0 && erb->mode != 7)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo1);
        glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    square_shader.useProgram();

    square_shader.setUniform1i("effect", erb->mode - 2);

    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    if (erb->mode == 7)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        square_shader.setUniform1i("texms", 0);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, texture1);
        square_shader.setUniform1i("tex", 0);
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);

    c.draw();
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void AntiAliasing::getKeyInput()
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
    if (getKeyPress(GLFW_KEY_A))
    {
        degree--;
    }
    if (getKeyPress(GLFW_KEY_D))
    {
        degree++;
    }
    if (direction)
    {
        camera.ProcessKeyboardInput(direction, delta);
    }
}

void AntiAliasing::createMultisampleFramebuffer()
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, WIDTH, HEIGHT, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "framebuffer create error" << std::endl;
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AntiAliasing::createFramebuffer()
{
    glGenFramebuffers(1, &fbo1);
    glGenTextures(1, &texture1);
    glGenRenderbuffers(1, &rbo1);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture1, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo1);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo1);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "framebuffer create error" << std::endl;
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AntiAliasing::createUniformBuffer()
{
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    after_create = true;
    updateUniformBuffer();
}

void AntiAliasing::createVAO()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float)));
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

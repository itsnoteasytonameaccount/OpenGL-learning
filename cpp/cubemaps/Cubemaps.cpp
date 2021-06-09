#include "Cubemaps.h"

static Camera camera(glm::vec3(0.0f));
static glm::mat4 projection;
static int WIDTH = 800, HEIGHT = 600;
static float degree = 0.0f;

static const char *const cubemaps_vertex_shader_path = "dist/cubemaps/shader/cubemaps.vs";
static const char *const cubemaps_fragment_shader_path = "dist/cubemaps/shader/cubemaps.fs";

static const char *const vertex_shader_path = "dist/cubemaps/shader/env_mapping.vs";
static const char *const fragment_shader_path = "dist/cubemaps/shader/env_mapping.fs";

static const char *const paths[] = {
    "dist/cubemaps/assets/right.jpg",
    "dist/cubemaps/assets/left.jpg",
    "dist/cubemaps/assets/top.jpg",
    "dist/cubemaps/assets/bottom.jpg",
    "dist/cubemaps/assets/front.jpg",
    "dist/cubemaps/assets/back.jpg"};

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

Cubemaps::Cubemaps(/* args */) : box(1)
{
    initWindow("立方体贴图测试场景", WIDTH, HEIGHT, _setViewport, _mouseCallback);

    shader.readFile(vertex_shader_path, fragment_shader_path);
    cubemaps_shader.readFile(cubemaps_vertex_shader_path, cubemaps_fragment_shader_path);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    Texture::LoadTexture("dist/framebuffers/assets/container.jpg", &texture1, 1, 0);
    Texture::LoadCubemaps(paths, &texture2, 1, 1);

    box.createVAO();
    m = new Model((char *)"dist/cubemaps/nanosuit/nanosuit.obj");
    c.initImGui(window);
}

void Cubemaps::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model(1.0f);

    cubemaps_shader.useProgram();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
    cubemaps_shader.setUniform1i("samplerTexture", 1);
    cubemaps_shader.setUniformMatrix4("view", glm::mat3(view));
    cubemaps_shader.setUniformMatrix4("projection", projection);
    box.bind();
    box.draw();

    shader.useProgram();
    if (c.mode == DRAW_MODEL)
    {
        model = glm::translate(model, glm::vec3(c.translate[0], c.translate[1], c.translate[2]));
        model = glm::scale(model, glm::vec3(c.scale[0], c.scale[1], c.scale[2]));
    }
    else
    {
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    }
    model = glm::rotate(model, glm::radians(degree), glm::vec3(0.0f, 1.0f, 0.0f));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture2);
    shader.setUniform1i("samplerTexture", 1);
    shader.setUniformMatrix3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));
    shader.setUniformMatrix4("model", model);
    shader.setUniformMatrix4("view", view);
    shader.setUniformMatrix4("projection", projection);
    shader.setUniform3fv("cameraPosition", camera.getPosition());
    shader.setUniform1i("mapping", c.mapping);
    if (c.mode == DRAW_MODEL)
    {
        m->draw(shader);
    }
    else if (c.mode == DRAW_BOX)
    {
        box.bind();
        box.draw();
    }
    c.dataChange();
}

Cubemaps::~Cubemaps()
{
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    delete m;
}

void Cubemaps::getKeyInput()
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
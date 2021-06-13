#include "HDR.h"

static int WIDTH = 800, HEIGHT = 600;
static Camera camera;
const char *const vertex_shader_path = "dist/HDR/shader/hdr.vs";
const char *const fragment_shader_path = "dist/HDR/shader/hdr.fs";

HDR::HDR(/* args */) : box(1)
{
    initWindow("hdr & 泛光", WIDTH, HEIGHT);
    shader.readFile(vertex_shader_path, fragment_shader_path);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    box.createVAO();
}

HDR::~HDR()
{
}

void HDR::getKeyInput()
{
    GLWindow::getKeyInput();
    camera.ProcessKeyboardInput(direction, delta);
}

void HDR::draw()
{
    glm::mat4 view = camera.getViewMatrix();
    float aspect = HEIGHT == 0 ? 0.0f : (float)WIDTH / HEIGHT;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 5.0f));
    shader.useProgram();
    shader.setUniformMatrix4("model", model);
    shader.setUniformMatrix4("view", view);
    shader.setUniformMatrix4("projection", projection);
    shader.setUniformMatrix4("normalMatrix", glm::inverse(glm::transpose(glm::mat3(view * model))));
    shader.setUniform3fv("texCoordTrans", glm::vec3(1.0f, 1.0f, 1.0f));
    box.bind();
    box.draw();
}

void HDR::createVAO()
{
}
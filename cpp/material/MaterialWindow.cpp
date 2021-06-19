#include "MaterialWindow.h"

static glm::mat4 projection;
static int WIDTH = 800, HEIGHT = 600;
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

static const char *vertex_shader_path = "dist/shader/material/vertex_shader.vs", *fragment_shader_path = "dist/shader/material/fragment_shader.fs", *light_fragment_shader_path = "dist/shader/material/light_fragment_shader.fs";

void _setViewport(GLFWwindow *window, int width, int height)
{
    projection = glm::perspective(glm::radians(camera.getZoom()), (float)width / height, 0.1f, 100.0f);
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

void mouseMove(GLFWwindow *window, double xpos, double ypos)
{
    camera.ProcessMouseMovement(xpos, ypos);
}

void scroll(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void MaterialWindow::getKeyInput()
{
    int direction = 0x00;
    if (getKeyPress(GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
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

MaterialWindow::MaterialWindow() : light_color(glm::vec3(1.0f)), light_pos(glm::vec3(1.2f, 1.0f, 2.0f)), specular(glm::vec3(0.5f, 0.5f, 0.5f)), ambient(glm::vec3(1.0f, 0.5f, 0.31f)), diffuse(glm::vec3(1.0f, 0.5f, 0.31f)), cube(1), shininess(32.0f)
{
    initWindow("材质场景", WIDTH, HEIGHT, _setViewport, mouseMove, scroll);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    object_shader.readFile(vertex_shader_path, fragment_shader_path);
    light_shader.readFile(vertex_shader_path, light_fragment_shader_path);
    light_shader.useProgram();

    cube.createVAO();
}

MaterialWindow::~MaterialWindow()
{
}

void MaterialWindow::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model(1.0f);
    glm::mat4 view = camera.getViewMatrix();

    light_color.x = sin(getTime() * 2.0f);
    light_color.y = sin(getTime() * 0.7f);
    light_color.z = sin(getTime() * 1.3f);

    object_shader.useProgram();
    object_shader.setUniform3fv("light.position", glm::vec3(view * glm::vec4(light_pos, 1.0f)));
    object_shader.setUniform3fv("light.ambient", light_color * glm::vec3(0.2f));
    object_shader.setUniform3fv("light.diffuse", light_color * glm::vec3(0.5f));
    object_shader.setUniform3fv("light.specular", glm::vec3(1.0f));
    object_shader.setUniform3fv("material.ambient", ambient);
    object_shader.setUniform3fv("material.diffuse", diffuse);
    object_shader.setUniform3fv("material.specular", specular);
    object_shader.setUniform1f("material.shininess", shininess);
    object_shader.setUniformMatrix4("projection", projection);
    object_shader.setUniformMatrix4("model", model);
    object_shader.setUniformMatrix4("view", view);
    object_shader.setUniformMatrix3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(view * model))));
    cube.draw();

    light_shader.useProgram();
    light_shader.setUniformMatrix4("projection", projection);
    light_shader.setUniformMatrix4("model", glm::translate(model, light_pos));
    light_shader.setUniformMatrix4("view", view);
    cube.draw();
}
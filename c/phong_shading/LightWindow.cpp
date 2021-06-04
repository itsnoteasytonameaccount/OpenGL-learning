#include "LightWindow.h"

static const char *const vertex_shader_path = "dist/shader/phong_shading/vertex_shader.vs";
static const char *const fragment_shader_path = "dist/shader/phong_shading/fragment_shader.fs";
static const char *const light_fragment_shader_path = "dist/shader/phong_shading/light_fragment_shader.fs";
static int WIDTH = 800, HEIGHT = 600;
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
static glm::mat4 projection;

static void _setViewport(GLFWwindow *window, int width, int height)
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

LightWindow::LightWindow(/* args */) : light_color(glm::vec3(1.0f, 1.0f, 1.0f)), light_pos(glm::vec3(0.0f, 1.0f, 0.0f)), object_color(glm::vec3(1.0f, 0.5f, 0.31f)), cube(1)
{
    initWindow("光照场景", WIDTH, HEIGHT, _setViewport, mouseMove, scroll);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    object_shader.readFile(vertex_shader_path, fragment_shader_path);
    light_shader.readFile(vertex_shader_path, light_fragment_shader_path);
    light_shader.useProgram();

    cube.createVAO();
}

LightWindow::~LightWindow()
{
}

void LightWindow::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat3 normal_model = glm::transpose(glm::inverse(glm::mat3(view * model)));

    // light_pos.y = sin(getTime()/2) * 3;
    // light_pos.x = cos(getTime()/2) * sin(getTime()/2) * 3;
    // light_pos.z = cos(getTime()/2) * cos(getTime()/2) * 3;

    object_shader.useProgram();

    object_shader.setUniformMatrix4("model", model);
    object_shader.setUniformMatrix4("projection", projection);
    object_shader.setUniformMatrix4("view", view);
    object_shader.setUniformMatrix3("normalModel", normal_model);
    object_shader.setUniform3fv("lightPos", glm::vec3(view * glm::vec4(light_pos, 1.0f)));
    object_shader.setUniform3fv("lightColor", light_color);
    object_shader.setUniform3fv("objectColor", object_color);
    object_shader.setUniform3fv("viewPos", camera.getPosition());
    cube.draw();

    model = glm::translate(glm::mat4(1.0f), light_pos);
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    light_shader.useProgram();
    light_shader.setUniformMatrix4("model", model);
    light_shader.setUniformMatrix4("projection", projection);
    light_shader.setUniformMatrix4("view", view);
    cube.draw();
}

void LightWindow::getKeyInput()
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
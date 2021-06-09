#include "ModelLoading.h"

static Camera camera;
static int WIDTH = 800, HEIGHT = 600;
static char *vertex_shader_path = "dist/shader/model_loading/model_loading.vs",
            *fragment_shader_path = "dist/shader/model_loading/model_loading.fs",
            *light_fragment_shader_path = "dist/shader/model_loading/model_loading_light.fs";
static glm::mat4 projection;
static float degree = 0.0f;
#define PI 3.14159265358979

/**
 * @param {char*} dst 目标数组
 * @param {char*} src 源数据
 * @param {int} start 复制到目标数组的start位置之后
 * @return {int} 返回复制的字符串长度
 */
int str_copy(char *dst, char const *src, int start)
{
    char *p1 = dst + start;
    char const *p2 = src;
    while (*p2 != 0)
    {
        *p1++ = *p2++;
    }
    *p1 = 0;
    return p1 - dst;
}

void _setViewport(GLFWwindow *window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    projection = glm::perspective(glm::radians(45.0f), height == 0 ? 0 : (float)width / height, 0.1f, 300.0f);
    glViewport(0, 0, width, height);
}

void onMousemove(GLFWwindow *window, double xpos, double ypos)
{
    camera.ProcessMouseMovement(xpos, ypos);
}

void onScroll(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

ModelLoading::ModelLoading(/* args */) : shininess(32.0f), light_position(glm::vec4(1.2f, 1.0f, 2.0f, 1.0f)), light_direction(glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f)), light_color(glm::vec3(1.0f)), c(new IsNormalShow())
{
    initWindow("模型加载场景", WIDTH, HEIGHT, _setViewport, onMousemove, onScroll);
    model = new Model("dist/IronMan/IronMan.obj");
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glClearColor(0, 0, 0, 1.0f);

    box = new Box(2);
    box->createVAO(true);

    object_shader.readFile(vertex_shader_path, fragment_shader_path, "dist/shader/model_loading/model_loading.gs");
    normal_shader.readFile(vertex_shader_path, "dist/shader/model_loading/model_loading_normal.fs", "dist/shader/model_loading/model_loading_normal.gs");
    light_shader.readFile(vertex_shader_path, light_fragment_shader_path);

    c.initImGui(window);
}

ModelLoading::~ModelLoading()
{
    delete model;
    delete box;
}

void ModelLoading::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 model(1.0f);
    glm::mat4 view = camera.getViewMatrix();

    object_shader.useProgram();
    model = glm::scale(model, glm::vec3(0.01f));
    model = glm::translate(model, glm::vec3(0.0f, -100.0f, 0.0f));
    model = model * glm::rotate(glm::mat4(1.0f), glm::radians(degree), glm::vec3(0.0f, 1.0f, 0.0f));
    object_shader.setUniformMatrix4("model", model);
    object_shader.setUniformMatrix4("view", view);
    object_shader.setUniformMatrix4("projection", projection);
    object_shader.setUniformMatrix3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(view * model))));
    object_shader.setUniform1f("time", ((IsNormalShow *)c.drawer)->mode == 2 ? getTime() : PI * 3 / 2);
    object_shader.setUniform1f("material.shininess", shininess);
    setLight("pointLight", 0, view * light_position, light_color, POINT_LIGHT);
    setLight("spotLight", 0, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), light_color, SPOT_LIGHT);
    setLight("dirLight", 0, view * light_direction, light_color, DIR_LIGHT);

    // box->bind(object_shader, "material.texture_diffuse1", "material.texture_specular1");
    // box->draw();
    this->model->draw(object_shader);

    normal_shader.useProgram();
    normal_shader.setUniformMatrix4("model", model);
    normal_shader.setUniformMatrix4("view", view);
    normal_shader.setUniformMatrix4("projection", projection);
    normal_shader.setUniformMatrix3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(view * model))));
    if (((IsNormalShow *)c.drawer)->mode == 1)
        this->model->draw(normal_shader);

    light_shader.useProgram();
    model = glm::translate(glm::mat4(1.0f), glm::vec3(light_position));
    model = glm::scale(model, glm::vec3(0.1f));
    light_shader.setUniformMatrix4("model", model);
    light_shader.setUniformMatrix4("view", view);
    light_shader.setUniformMatrix4("projection", projection);
    light_shader.setUniform3f("color", 1.0f, 1.0f, 1.0f);

    box->bind();
    box->draw();

    c.draw();
}

void ModelLoading::setLight(char const *name, char index, glm::vec4 vector, glm::vec3 color, int type)
{
    int len = str_copy(param, name, 0);
    float ambient = 0.2, diffuse = 0.5, specular = 1.0f;
    if (index)
    {
        param[len++] = '[';
        param[len++] = index;
        param[len++] = ']';
    }
    param[len++] = '.';

    str_copy(param, "specular", len);
    object_shader.setUniform3f(param, color.x, color.y, color.z);

    str_copy(param, "linear", len);
    object_shader.setUniform1f(param, 0.09f);
    str_copy(param, "quadratic", len);
    object_shader.setUniform1f(param, 0.032f);
    str_copy(param, "constant", len);
    object_shader.setUniform1f(param, 1.0f);

    // object_shader.setUniform4f(param, )
    switch (type)
    {
    case DIR_LIGHT:
        str_copy(param, "cutOff", len);
        object_shader.setUniform1f(param, -1.0f);
        ambient = 0.05f;
        diffuse = 0.4f;
        specular = 0.5f;

        str_copy(param, "vector", len);
        object_shader.setUniform4f(param, vector.x, vector.y, vector.z, vector.w);
        break;
    case SPOT_LIGHT:
        str_copy(param, "vector", len);
        object_shader.setUniform4f(param, vector.x, vector.y, vector.z, 1.0f);
        str_copy(param, "direction", len);
        object_shader.setUniform4f(param, 0.0f, 0.0f, -1.0f, 0.0f);

        ambient = 0.0f;
        diffuse = 1.0f;
        specular = 1.0f;

        str_copy(param, "cutOff", len);
        object_shader.setUniform1f(param, glm::cos(glm::radians(12.5f)));
        str_copy(param, "outerCutOff", len);
        object_shader.setUniform1f(param, glm::cos(glm::radians(17.5f)));
        break;
    case POINT_LIGHT:
        str_copy(param, "cutOff", len);
        object_shader.setUniform1f(param, -1.0f);

        ambient = 0.05f;
        diffuse = 0.8f;
        specular = 1.0f;

        str_copy(param, "vector", len);
        object_shader.setUniform4f(param, vector.x, vector.y, vector.z, 1.0f);
        break;
    default:
        break;
    }

    str_copy(param, "ambient", len);
    object_shader.setUniform3f(param, ambient * color.x, ambient * color.y, ambient * color.z);
    str_copy(param, "diffuse", len);
    object_shader.setUniform3f(param, diffuse * color.x, diffuse * color.y, diffuse * color.z);
}

void ModelLoading::getKeyInput()
{
    int direction = 0x00;
    if (getKeyPress(GLFW_KEY_ESCAPE, GLFW_PRESS))
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
#include "LightCasters.h"

static Camera camera;
static int WIDTH = 800, HEIGHT = 600;
static glm::mat4 projection;

const char *vertex_shader_path = "dist/shader/light_casters/light_casters.vs",
           *fragment_shader_path = "dist/shader/light_casters/light_casters.fs",
           *light_fragment_shader_path = "dist/shader/light_casters/light_casters_light.fs",
           *texture_path = "dist/assets/light_casters/container2.png",
           *specular_texture_path = "dist/assets/light_casters/container2_specular.png";

static glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f, 2.0f, -12.0f),
    glm::vec3(0.0f, 0.0f, -3.0f)};

static glm::vec3 cube_positions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};

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

LightCasters::LightCasters(/* args */) : box(1), light_color(glm::vec4(1.0f)), shininess(32.0f), light_pos(glm::vec4(1.2f, 1.0f, 2.0f, 1.0f)), light_direction(glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f))
{
    initWindow("平行光场景", WIDTH, HEIGHT, _setViewport, mouseMove, scroll);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    object_shader.readFile(vertex_shader_path, fragment_shader_path);
    light_shader.readFile(vertex_shader_path, light_fragment_shader_path);

    box.createVAO();

    texture0num = 0;
    texture1num = 1;
    Texture::LoadTexture(texture_path, &texture0, 1, texture0num);
    Texture::LoadTexture(specular_texture_path, &texture1, 1, texture1num);
}

LightCasters::~LightCasters()
{
}

void LightCasters::draw()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model(1.0f);
    glm::mat4 view = camera.getViewMatrix();

    // light_color.x = sin(getTime() * 2.0f);
    // light_color.y = sin(getTime() * 0.7f);
    // light_color.z = sin(getTime() * 1.3f);

    object_shader.useProgram();
    setLight("spotLight", 0, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), light_color, SPOT_LIGHT);
    setLight("dirLight", 0, view * light_direction, light_color, DIR_LIGHT);
    // object_shader.setUniform4fv("light.vector", view * light_pos);
    // object_shader.setUniform4fv("light.vector", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    // object_shader.setUniform4fv("light.direction", glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
    // object_shader.setUniform4fv("light.vector", view * light_direction);
    // object_shader.setUniform4fv("light.ambient", light_color * glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    // object_shader.setUniform4fv("light.diffuse", light_color * glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    // object_shader.setUniform1f("light.cutOff", glm::cos(glm::radians(12.5f)));
    // object_shader.setUniform1f("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    // object_shader.setUniform1f("light.constant", 1.0f);
    // object_shader.setUniform1f("light.linear", 0.09f);
    // object_shader.setUniform1f("light.quadratic", 0.032f);
    // object_shader.setUniform4fv("light.specular", light_color);
    for (int i = 0; i < 4; i++)
    {
        setLight("lights", (char)(i + 48), view * glm::vec4(pointLightPositions[i], 1.0f), light_color, POINT_LIGHT);
    }
    object_shader.setUniform1i("material.diffuse", texture0num);
    object_shader.setUniform1i("material.specular", texture1num);
    // object_shader.setUniform1i("emissionMaps", texture2num);
    object_shader.setUniform1f("material.shininess", shininess);
    object_shader.setUniformMatrix4("projection", projection);
    object_shader.setUniformMatrix4("view", view);

    box.bind();
    for (int i = 0; i < 10; i++)
    {
        model = glm::translate(glm::mat4(1.0f), cube_positions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        object_shader.setUniformMatrix4("model", model);
        object_shader.setUniformMatrix4("normalMatrix", glm::transpose(glm::inverse(view * model)));
        box.draw();
    }

    light_shader.useProgram();
    light_shader.setUniformMatrix4("projection", projection);
    light_shader.setUniformMatrix4("view", view);
    light_shader.setUniform4fv("lightColor", light_color);
    model = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
    box.bind();

    for (int i = 0; i < 4; i++)
    {
        light_shader.setUniformMatrix4("model", model);
        model = glm::translate(model, pointLightPositions[i]);
        box.draw();
    }
}

void LightCasters::setLight(char const *name, char index, glm::vec4 vector, glm::vec4 color, int type)
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
    object_shader.setUniform4f(param, color.x, color.y, color.z, 1.0f);

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
    object_shader.setUniform4f(param, ambient * color.x, ambient * color.y, ambient * color.z, 1.0f);
    str_copy(param, "diffuse", len);
    object_shader.setUniform4f(param, diffuse * color.x, diffuse * color.y, diffuse * color.z, 1.0f);
}

void LightCasters::getKeyInput()
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
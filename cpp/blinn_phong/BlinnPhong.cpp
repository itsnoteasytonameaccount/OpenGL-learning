#include "BlinnPhong.h"

Camera camera;
glm::mat4 projection;

const char *vertex_shader_path = "dist/blinn_phong/shader/blinn_phong.vs";
const char *fragment_shader_path = "dist/blinn_phong/shader/blinn_phong.fs";
static const char *const light_fragment_shader_path = "dist/shader/phong_shading/light_fragment_shader.fs";
static int WIDTH = 800, HEIGHT = 600;
static bool after_create = false;
static unsigned int ubo;
static bool view_lock = true;

const float square[] = {
    -10.0f,
    -0.5f,
    10.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,
    0.0f,

    10.0f,
    -0.5f,
    10.0f,
    0.0f,
    1.0f,
    0.0f,
    10.0f,
    0.0f,

    10.0f,
    -0.5f,
    -10.0f,
    0.0f,
    1.0f,
    0.0f,
    10.0f,
    10.0f,

    -10.0f,
    -0.5f,
    -10.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,
    10.0f,
};

const unsigned int indices[] = {
    0, 1, 2, 0, 2, 3};

void _mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    camera.ProcessMouseMovement(xpos, ypos);
}

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
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
    float aspect = height == 0 ? 0 : (float)width / height;
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    updateUniformBuffer();
}

BlinnPhong::BlinnPhong(/* args */) : light_color(1.0f, 1.0f, 1.0f), box(1), c(new Adjust())
{
    initWindow("blinn-phong", WIDTH, HEIGHT, _setViewport, _mouseCallback);
    shader.readFile(vertex_shader_path, fragment_shader_path);
    light_shader.readFile(vertex_shader_path, light_fragment_shader_path);
    shader.setUniformBlockBinding("Matrices", 0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    createVAO();
    box.createVAO();
    Texture::LoadTexture("dist/blinn_phong/assets/wood.png", &texture);
    c.initImGui(window);
}

BlinnPhong::~BlinnPhong()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &ubo);
}

void BlinnPhong::createVAO()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &ubo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, sizeof(glm::mat4) * 2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    after_create = true;
    updateUniformBuffer();
}

void BlinnPhong::draw()
{
    Adjust *adjust = (Adjust *)c.getDrawer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 model(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(view * model)));
    char param[20];

    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
    model = glm::rotate(model, glm::radians(adjust->degree), glm::vec3(1.0f, 0.0f, 0.0f));

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    shader.useProgram();
    glBindVertexArray(vao);
    shader.setUniformMatrix3("normalMatrix", normalMatrix);
    shader.setUniformMatrix4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setUniform1i("material.tex", 0);
    shader.setUniform1f("material.shininess", adjust->shininess);
    shader.setUniform1i("mode", adjust->mode);
    shader.setUniform1i("quadratic", adjust->quadratic);
    shader.setUniform1f("gamma", adjust->gamma);
    for (int i = 0; i < 4; i++)
    {
        strcpy(param, "lights[0].position");
        param[7] = i + '0';
        shader.setUniform3fv(param, glm::vec3(view * glm::vec4(adjust->lights[i].light_pos, 1.0f)));
        strcpy(param, "lights[0].ambient");
        param[7] = i + '0';
        shader.setUniform3fv(param, light_color * glm::vec3(adjust->lights[i].ambient));
        strcpy(param, "lights[0].diffuse");
        param[7] = i + '0';
        shader.setUniform3fv(param, light_color * glm::vec3(adjust->lights[i].diff));
        strcpy(param, "lights[0].specular");
        param[7] = i + '0';
        shader.setUniform3fv(param, light_color * glm::vec3(adjust->lights[i].spec));
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    light_shader.useProgram();
    for (int i = 0; i < 4; i++)
    {
        model = glm::translate(glm::mat4(1.0f), adjust->lights[i].light_pos);
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
        light_shader.setUniformMatrix4("model", model);
    }
    // box.bind();
    // box.draw();

    adjust->camera_pos = camera.getPosition();
    c.draw();
}

void BlinnPhong::getKeyInput()
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
        ((Adjust *)c.getDrawer())->degree--;
    }
    if (getKeyPress(GLFW_KEY_D))
    {
        ((Adjust *)c.getDrawer())->degree++;
    }
    if (direction)
    {
        camera.ProcessKeyboardInput(direction, delta);
    }
}
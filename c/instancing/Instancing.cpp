#include "Instancing.h"

static const char *const vertex_shader_path = "dist/instancing/shader/instancing.vs";
static const char *const fragment_shader_path = "dist/instancing/shader/instancing.fs";
static int WIDTH = 800, HEIGHT = 600;
static glm::mat4 projection;
static Camera camera(glm::vec3(0.0f, 0.0f, 155.0f));
static float degree = 0.0f;
static unsigned int ubo;
static bool after_create = false;

float quadVertices[] = {
    // 位置          // 颜色
    -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
    0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

    -0.05f, 0.05f, 1.0f, 0.0f, 0.0f,
    0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
    0.05f, 0.05f, 0.0f, 1.0f, 1.0f};

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
    projection = glm::perspective(glm::radians(camera.getZoom()), (float)aspect, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);
    // updateUniformBuffer();
}

void _mouseCallback(GLFWwindow *window, double xpos, double ypos)
{
    camera.ProcessMouseMovement(xpos, ypos);
}

Instancing::Instancing() : GLWindow(WIDTH, HEIGHT, _setViewport), con(new AmountCon())
{
    this->mouseCallback = _mouseCallback;
    this->scrollCallback = NULL;
    initWindow(WIDTH, HEIGHT, "实例化");
    con.initImGui(window);

    shader.readFile(vertex_shader_path, fragment_shader_path);
    planet_shader.readFile("dist/instancing/shader/planet_instancing.vs", "dist/instancing/shader/planet_instancing.fs");
    // createUniformBuffer();
    srand(getTime());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);

    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2)
    {
        for (int x = -10; x < 10; x += 2)
        {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            offsets[index++] = translation;
        }
    }

    generateModelMatrixList();
    createVAO();

    planet = new Model("dist/instancing/planet/planet.obj");
    rock = new Model("dist/instancing/rock/rock.obj");

    bindMeshVAO();
}

Instancing::~Instancing()
{
    delete planet;
    delete rock;
    delete (AmountCon *)con.getDrawer();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &vbo2);
    glDeleteBuffers(1, &vbo3);
    glDeleteBuffers(1, &ubo);

    free(models);
}

void Instancing::createVAO()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &vbo2);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, offsets, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glVertexAttribDivisor(2, 1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Instancing::createUniformBuffer()
{
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, 2 * sizeof(glm::mat4));
    after_create = true;
    updateUniformBuffer();
}

void Instancing::draw()
{
    AmountCon *ac = (AmountCon *)con.getDrawer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (current_amount < ac->amount)
    {
        models = (glm::mat4 *)realloc(models, ac->amount * sizeof(glm::mat4));
        for (unsigned int j = current_amount; j < ac->amount; j++)
        {
            models[j] = generateModelMatrix(150.0f, 25.0f, (float)(rand() % ac->amount) / ac->amount * 360.0f);
        }
        current_amount = ac->amount;

        glBindBuffer(GL_ARRAY_BUFFER, vbo3);
        // 会删除之前的数据
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * current_amount, models, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    if (ac->example == 0)
    {
        shader.useProgram();
        glBindVertexArray(vao);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
    }
    else
    {
        planet_shader.useProgram();
        planet_shader.setUniformMatrix4("view", camera.getViewMatrix());
        planet_shader.setUniformMatrix4("projection", projection);
        planet_shader.setUniformMatrix4("model", glm::scale(glm::mat4(1.0f), glm::vec3(4.0f)));
        planet_shader.setUniform1i("type", 1);
        planet->draw(planet_shader);
        if (ac->example == 1)
        {
            planet_shader.setUniform1i("type", 1);
            for (unsigned int i = 0; i < ac->amount; i++)
            {
                planet_shader.setUniformMatrix4("model", models[i]);
                rock->draw(planet_shader);
            }
        }
        else
        {
            planet_shader.setUniform1i("type", 2);
            rock->draw(planet_shader, ac->amount);
        }
    }
    con.draw();
}

void Instancing::getKeyInput()
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

void Instancing::generateModelMatrixList()
{
    AmountCon *ac = (AmountCon *)con.getDrawer();
    models = (glm::mat4 *)calloc(ac->amount, sizeof(glm::mat4));
    float radius = 150.0f;
    float offset = 25.0f;
    current_amount = ac->amount;
    for (unsigned int i = 0; i < ac->amount; i++)
    {
        // 4. 添加到矩阵的数组中
        models[i] = generateModelMatrix(radius, offset, (float)i / ac->amount * 360.0f);
    }
}

glm::mat4 Instancing::generateModelMatrix(float radius, float offset, float angle)
{
    glm::mat4 model(1.0f);
    float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float x = sin(angle) * radius + displacement;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float y = 0.4f * displacement;
    displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
    float z = cos(angle) * radius + displacement;
    model = glm::translate(model, glm::vec3(x, y, z));

    float scale = (rand() % 20) / 100.0f + 0.05f;
    model = glm::scale(model, glm::vec3(scale));

    float rotAngle = (rand() % 360);
    model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
    return model;
}

void Instancing::bindMeshVAO()
{
    glGenBuffers(1, &vbo3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * current_amount, models, GL_STATIC_DRAW);
    unsigned int num = rock->getMeshNum();

    for (unsigned int i = 0; i < num; i++)
    {
        glBindVertexArray(rock->getMeshVAO(i));
        GLsizei vec4size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, 0);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void *)(1 * vec4size));
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void *)(2 * vec4size));
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4size, (void *)(3 * vec4size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
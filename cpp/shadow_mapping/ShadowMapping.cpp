#include "ShadowMapping.h"

Camera camera;
glm::mat4 projection;

const char *const vertex_shader_path = "dist/shadow_mapping/shader/shadow_mapping.vs";
const char *const fragment_shader_path = "dist/shadow_mapping/shader/shadow_mapping.fs";
const char *const light_space_fragment_shader_path = "dist/shadow_mapping/shader/shadow.fs";
const char *const light_space_vertex_shader_path = "dist/shadow_mapping/shader/shadow.vs";
;
static int WIDTH = 800, HEIGHT = 600;
static int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
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

ShadowMapping::ShadowMapping(/* args */) : light_color(1.0f, 1.0f, 1.0f), box(1), c(new Adjust()), light_direction(0.0f, -1.0f, 0.0f)
{
    initWindow("shadow mapping", WIDTH, HEIGHT, _setViewport, _mouseCallback);
    shader.readFile(vertex_shader_path, fragment_shader_path);
    shadow_shader.readFile(light_space_vertex_shader_path, light_space_fragment_shader_path);
    shader.setUniformBlockBinding("Matrices", 0);
    sm.init();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    createVAO();
    createShadowFramebuffer();
    box.createVAO();
    Texture::LoadTexture("dist/blinn_phong/assets/wood.png", &texture);
    c.initImGui(window);
}
ShadowMapping::~ShadowMapping()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &shadow_texel);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &ubo);
}

void ShadowMapping::draw()
{
    Adjust *adjust = (Adjust *)c.getDrawer();
    adjust->camera_pos = camera.getPosition();

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    // glm::mat4 light_projection = glm::ortho(-4.0f, 4.0f, -10.0f, 10.0f, adjust->near, adjust->far);
    glm::mat4 light_projection = glm::perspective(glm::radians(45.0f), 1.0f, adjust->near, adjust->far);
    glm::mat4 light_view = glm::lookAt(adjust->light->position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 light_space_matrix = light_projection * light_view;
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 model(1.0f);

    shadow_shader.useProgram();
    shadow_shader.setUniformMatrix4("lightSpaceMatrix", light_space_matrix);
    // shadow_shader.setUniformMatrix4("projection", light_projection);
    // shadow_shader.setUniformMatrix4("view", light_view);
    box.bind();
    for (int i = 0; i < 3; i++)
    {
        model = glm::translate(glm::mat4(1.0f), adjust->box_pos[i]);
        shadow_shader.setUniformMatrix4("model", model);
        box.draw();
    }

    model = glm::mat4(1.0f);
    shadow_shader.setUniformMatrix4("model", model);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glViewport(0, 0, WIDTH, HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glCullFace(GL_BACK);

    shader.useProgram();

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(light_space_matrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadow_texel);
    shader.setUniform1i("material.tex", 0);
    shader.setUniform1i("shadow_texel", 1);
    shader.setUniform1f("material.shininess", adjust->shininess);

    shader.setUniform3fv("light.ambient", glm::vec3(adjust->light->ambient));
    shader.setUniform3fv("light.specular", glm::vec3(adjust->light->spec));
    shader.setUniform3fv("light.diffuse", glm::vec3(adjust->light->diff));
    shader.setUniform3fv("light.position", glm::vec3(view * glm::vec4(adjust->light->position, 1.0f)));

    box.bind();
    for (int i = 0; i < 3; i++)
    {
        model = glm::translate(glm::mat4(1.0f), adjust->box_pos[i]);
        shader.setUniformMatrix4("model", model);
        shader.setUniformMatrix3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(view * model))));
        box.draw();
    }

    model = glm::mat4(1.0f);
    shader.setUniformMatrix4("model", model);
    shader.setUniformMatrix3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(view * model))));
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    sm.draw(shadow_texel);

    c.draw();
}

void ShadowMapping::createVAO()
{
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &ubo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 3, NULL, GL_STATIC_DRAW);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, sizeof(glm::mat4) * 3);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    after_create = true;
    updateUniformBuffer();
}

void ShadowMapping::createShadowFramebuffer()
{
    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &shadow_texel);

    glBindTexture(GL_TEXTURE_2D, shadow_texel);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = {1.0, 1.0, 1.0, 1.0};
    // 设置边框颜色
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_texel, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMapping::getKeyInput()
{
    Adjust *adjust = (Adjust *)c.getDrawer();
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
        adjust->degree--;
    }
    if (getKeyPress(GLFW_KEY_D))
    {
        adjust->degree++;
    }
    if (direction)
    {
        camera.ProcessKeyboardInput(direction, delta);
    }
}
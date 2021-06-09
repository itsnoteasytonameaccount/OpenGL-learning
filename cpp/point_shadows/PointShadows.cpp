#include "PointShadows.h"

static unsigned int ubo;
static Camera camera;
static glm::mat4 projection;
static int WIDTH = 800, HEIGHT = 600;

const int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
const float near = 1.0f;
const float far = 25.0f;
static bool after_create = false;
static const char *const paths[] = {
    "dist/cubemaps/assets/right.jpg",
    "dist/cubemaps/assets/left.jpg",
    "dist/cubemaps/assets/top.jpg",
    "dist/cubemaps/assets/bottom.jpg",
    "dist/cubemaps/assets/front.jpg",
    "dist/cubemaps/assets/back.jpg"};

const char *const vertex_shader_path = "dist/point_shadows/shader/point_shadows.vs";
const char *const fragment_shader_path = "dist/point_shadows/shader/point_shadows.fs";
const char *const light_space_fragment_shader_path = "dist/point_shadows/shader/point_shadows_depth.fs";
const char *const light_space_vertex_shader_path = "dist/point_shadows/shader/point_shadows_depth.vs";
const char *const light_space_geometry_shader_path = "dist/point_shadows/shader/point_shadows_depth.gs";

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

PointShadows::PointShadows(/* args */) : box(1), c(new Adjust())
{
    initWindow("point shadows", WIDTH, HEIGHT, _setViewport, _mouseCallback);
    shader.readFile(vertex_shader_path, fragment_shader_path);
    shadow_shader.readFile(light_space_vertex_shader_path, light_space_fragment_shader_path, light_space_geometry_shader_path);
    shader.setUniformBlockBinding("Matrices", 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearDepth(1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    prepare();
    createFramebuffer();
    box.createVAO();
    Texture::LoadTexture("dist/blinn_phong/assets/wood.png", &texture);
    // Texture::LoadCubemaps(paths, &depth_texture, 1, 1);
    c.initImGui(window);
    dw.init();
}

PointShadows::~PointShadows()
{
    glDeleteTextures(1, &texture);
    glDeleteFramebuffers(1, &fbo);
    glDeleteBuffers(1, &ubo);
}

void PointShadows::createFramebuffer()
{
    glGenFramebuffers(1, &fbo);

    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depth_texture);
    for (unsigned int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void PointShadows::prepare()
{
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);

    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, sizeof(glm::mat4) * 2);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    after_create = true;
    updateUniformBuffer();
}

void PointShadows::draw()
{
    glm::mat4 view = camera.getViewMatrix();
    Adjust *adjust = (Adjust *)c.getDrawer();
    glm::mat4 shadow_matrices[6];
    glm::vec3 light_pos = adjust->light->position;

    glm::mat4 shadow_proj = glm::perspective(glm::radians(90.0f), aspect, near, far);
    shadow_matrices[0] = shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    shadow_matrices[1] = shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    shadow_matrices[2] = shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    shadow_matrices[3] = shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
    shadow_matrices[4] = shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
    shadow_matrices[5] = shadow_proj * glm::lookAt(light_pos, light_pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
    shadow_shader.useProgram();
    shadow_shader.setUniform3fv("lightPos", light_pos);
    shadow_shader.setUniform1f("far_plane", far);
    char matrices_name[20] = "shadow_matrices[0]";
    for (unsigned int i = 0; i < 6; i++)
    {
        matrices_name[16] = 48 + i;
        shadow_shader.setUniformMatrix4(matrices_name, shadow_matrices[i]);
    }
    renderScence(shadow_shader);

    glViewport(0, 0, WIDTH, HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.useProgram();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depth_texture);
    shader.setUniform1i("material.tex", 0);
    shader.setUniform1f("material.shininess", adjust->shininess);
    shader.setUniform3fv("light.position", adjust->light->position);
    shader.setUniform3fv("light.specular", glm::vec3(adjust->light->spec));
    shader.setUniform3fv("light.ambient", glm::vec3(adjust->light->ambient));
    shader.setUniform3fv("light.diffuse", glm::vec3(adjust->light->diff));
    shader.setUniform1i("shadow_texel", 1);
    shader.setUniform1f("far_plane", far);
    shader.setUniform3fv("light_pos", camera.getPosition());

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    renderScence(shader);

    adjust->camera_pos = camera.getPosition();
    dw.draw(projection, view, depth_texture, WIDTH, HEIGHT, adjust->scale);
    c.draw();
}

void PointShadows::renderScence(Shader &shader)
{
    box.bind();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0f));
    shader.setUniformMatrix4("model", model);
    glDisable(GL_CULL_FACE);
    shader.setUniform1i("reverse_normal", 1);
    box.draw();
    shader.setUniform1i("reverse_normal", 0);
    glEnable(GL_CULL_FACE);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
    model = glm::scale(model, glm::vec3(1.0f));
    shader.setUniformMatrix4("model", model);
    box.draw();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
    model = glm::scale(model, glm::vec3(1.5f));
    shader.setUniformMatrix4("model", model);
    box.draw();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(1.0f));
    shader.setUniformMatrix4("model", model);
    box.draw();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
    model = glm::scale(model, glm::vec3(1.0f));
    shader.setUniformMatrix4("model", model);
    box.draw();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(1.5f));
    shader.setUniformMatrix4("model", model);
    box.draw();
}

void PointShadows::getKeyInput()
{
    // Adjust *adjust = (Adjust *)c.getDrawer();
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
    // if (getKeyPress(GLFW_KEY_A))
    // {
    //     adjust->degree--;
    // }
    // if (getKeyPress(GLFW_KEY_D))
    // {
    //     adjust->degree++;
    // }
    if (direction)
    {
        camera.ProcessKeyboardInput(direction, delta);
    }
}
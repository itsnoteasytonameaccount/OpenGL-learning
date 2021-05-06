#include "CSM.h"

int WIDTH = 800, HEIGHT = 600;
const float near = 0.1f, far = 100.0f;
static Camera camera;
static glm::mat4 projection;

const char *const vertex_shader_path = "dist/point_shadows/shader/point_shadows.vs";
const char *const fragment_shader_path = "dist/point_shadows/shader/point_shadows.fs";
const char *const light_space_fragment_shader_path = "dist/point_shadows/shader/point_shadows_depth.fs";
const char *const light_space_vertex_shader_path = "dist/point_shadows/shader/point_shadows_depth.vs";
const glm::vec4 normalized_pos[8] = {
    glm::vec4(1.0f),
    glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, -1.0f, 1.0f, 1.0f),
    glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f),
    glm::vec4(1.0f, 1.0f, -1.0f, 1.0f),
    glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),
    glm::vec4(1.0f, -1.0f, -1.0f, 1.0f),
    glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f)};

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

void _setViewport(GLFWwindow *window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
    float aspect = height == 0 ? 0 : (float)width / height;
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

CSM::CSM(/* args */) : GLWindow(WIDTH, HEIGHT, _setViewport), box(1)
{
    this->mouseCallback = _mouseCallback;
    this->scrollCallback = NULL;
    initWindow("cascaded shadow maps");
    shader.readFile(vertex_shader_path, fragment_shader_path);
    shadow_shader.readFile(light_space_vertex_shader_path, light_space_fragment_shader_path);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Texture::LoadTexture("dist/blinn_phong/assets/wood.png", &texture);

    box.createVAO();
    createVAO();
    createModels();
    bindUniform();
    createFramebuffer();
}

CSM::~CSM()
{
    glDeleteFramebuffers(1, &fbo);
    glDeleteBuffers(1, &vbo);
}

void CSM::updateSplitZ(float N)
{
    const float lambda = 0.5;
    float log_split, uniform_split;
    for (int i = 0; i < 3; i++)
    {
        log_split = near * math.pow(far / near, (float)i / N);
        uniform_split = near + (far - near) * (i / N);
        z_values[i] = lambda * log_split + (1.0f - lambda) * uniform_split;
    }
}

void CSM::computeLightProjection(int i, float aspect, glm::mat4 &view_inverse)
{
    float minz, maxz, minx, maxx, miny, maxy;
    glm::vec4 light_space_pos;
    glm::mat4 projection_inverse = glm::inverse(glm::perspective(glm::radians(45.0f), aspect, near, far));
    glm::vec4 z_e(-z_values[i]);
    for (int j = 0; j < 8; j++)
    {
        light_space_pos = light_view * view_inverse * projection_inverse * (z_e * normalized_pos[j]);
        if (j == 0)
        {
            minz = light_space_pos.z;
            minx = light_space_pos.x;
            miny = light_space_pos.y;
            maxz = light_space_pos.z;
            maxx = light_space_pos.x;
            maxy = light_space_pos.y;
        }
        else
        {
            if (light_space_pos.z < minz)
            {
                minz = light_space_pos.z;
            }
            else if (light_space_pos.z > maxz)
            {
                maxz = light_space_pos.z;
            }
            if (light_space_pos.x < minx)
            {
                minx = light_space_pos.x;
            }
            else if (light_space_pos.x > maxx)
            {
                maxx = light_space_pos.x;
            }
            if (light_space_pos.y < miny)
            {
                miny = light_space_pos.y;
            }
            else if (light_space_pos.y > maxy)
            {
                maxy = light_space_pos.y;
            }
        }
    }
    light_projection[i] = glm::ortho(minx, maxx, miny, maxy, minz, maxz) * light_view;
}

void CSM::draw()
{
    float aspect = HEIGHT == 0 ? 0 : (float)WIDTH / HEIGHT;
    glm::mat4 view_inverse = glm::inverse(camera.getViewMatrix());

    light_view = glm::lookAt(-(light_direction * glm::vec3(10.0f)), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    updateSplitZ();

    glBindFramebuffer(GL_FRAMBUFFER, fbo);
    for (int i = 0; i < 3; i++)
    {
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_array, 0, i);
        glClear(GL_DEPTH_BUFFER_BIT);
        drawScence(shadow_shader);
        /* code */
    }

    glBindFramebuffer(GL_FRAMBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
    shader.setUniform1i("shadow_texel", 1);
    glActiveTexture(GL_TEXTURE0);
    drawScence(shader);
}

void CSM::createVAO()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CSM::drawScence(Shader &shader)
{
    shader.setUniformMatrix4("projection", projection);
    shader.setUniformMatrix4("view", camera.getViewMatrix());
    box.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE2D, texture);
    shader.setUniform1i("material.tex", 0);
    glBindTexture(GL_TEXTURE2D, 0);

    shader.setUniform1f("z_values[0]", z_values[0]);
    shader.setUniform1f("z_values[1]", z_values[1]);
    shader.setUniform1f("z_values[2]", z_values[2]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE3D, texture_array);
    shader.setUniform1i("shadow_texel", 1);
    glBindTexture(GL_TEXTURE3D, 0);

    shader.setUniform3fv("light.direction", light_direction);
    shader.setUniform3fv("light.specular", glm::vec3(1.0f) * glm::vec3(0.3f));
    shader.setUniform3fv("light.diffuse", glm::vec3(1.0f) * glm::vec3(1.0f));
    shader.setUniform3fv("light.ambient", glm::vec3(1.0f) * glm::vec3(0.05f));

    shader.setUniformMatrix4("light_projection_view[0]", light_projection_view[0]);
    shader.setUniformMatrix4("light_projection_view[1]", light_projection_view[1]);
    shader.setUniformMatrix4("light_projection_view[2]", light_projection_view[2]);

    shader.setUniform1i("use_uniform", 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100);

    shader.setUniform1i("use_uniform", 1);
    shader.setUniformMatrix4("model2", glm::mat4(1.0f));
    glBindVertexArray(vao);
    glDrawArrays(GL_TIRANGLES, 0, 36);
}

void CSM::bindUniform()
{
    GLsizei vec4size = sizeof(glm::vec4);
    glGenBuffers(1, &vbo);
    box.bind();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * 100, models, GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(3, 4, GL_FLOAT, 4 * vec4size, 0);
    glVertexAttribPointer(3, 4, GL_FLOAT, 4 * vec4size, ((void *)1 * vec4size));
    glVertexAttribPointer(3, 4, GL_FLOAT, 4 * vec4size, ((void *)2 * vec4size));
    glVertexAttribPointer(3, 4, GL_FLOAT, 4 * vec4size, ((void *)3 * vec4size));

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glBindVertexArrray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CSM::createModels()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            models[i * 10 + j] = glm::translate(glm::mat4(1.0f), glm::vevc3((i - 5) * 2 + 1, (j - 5) * 2 + 1, 0.0f));
        }
    }
}

void CSM::createFramebuffer()
{
    glGenTextures(1, texture_array);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, 800, 800, 3, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    glGenFramebffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CSM::createVAO()
{
    glGenVertexArray(1, &vao);
    glGenBuffers(1, &vbo);
}

void CSM::getKeyInput()
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
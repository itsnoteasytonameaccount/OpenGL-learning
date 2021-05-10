#include "CSM.h"

int WIDTH = 800, HEIGHT = 600;
const float near = 0.1f, far = 30.0f;
static Camera camera;
static glm::mat4 projection;

const char *const vertex_shader_path = "dist/csm/shader/csm.vs";
const char *const fragment_shader_path = "dist/csm/shader/csm.fs";
const char *const light_space_fragment_shader_path = "dist/csm/shader/csm_shadow.fs";
const char *const light_space_vertex_shader_path = "dist/csm/shader/csm_shadow.vs";
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
    10.0f,

    10.0f,
    -0.5f,
    10.0f,
    0.0f,
    1.0f,
    0.0f,
    10.0f,
    10.0f,

    10.0f,
    -0.5f,
    -10.0f,
    0.0f,
    1.0f,
    0.0f,
    10.0f,
    0.0f,

    -10.0f,
    -0.5f,
    -10.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,
    0.0f,
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
    projection = glm::perspective(glm::radians(45.0f), aspect, near, far);
}

CSM::CSM(/* args */) : GLWindow(WIDTH, HEIGHT, _setViewport), box(1), light_direction(1.0f, -0.1f, -1.0f)
{
    this->mouseCallback = _mouseCallback;
    this->scrollCallback = NULL;
    initWindow("cascaded shadow maps");
    shader.readFile(vertex_shader_path, fragment_shader_path);
    shadow_shader.readFile(light_space_vertex_shader_path, light_space_fragment_shader_path);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    Texture::LoadTexture("dist/blinn_phong/assets/wood.png", &texture);

    box.createVAO();
    createVAO();
    createModels();
    bindUniform();
    createFramebuffer();
    sw.init();
}

CSM::~CSM()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &texture_array);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &model_vbo);
    glDeleteBuffers(1, &vbo);
}

void CSM::updateSplitZ(int N)
{
    const float lambda = 0.5;
    float log_split, uniform_split;
    for (int i = 1; i <= 3; i++)
    {
        log_split = near * pow(far / near, (float)i / N);
        uniform_split = near + (far - near) * ((float)i / N);
        z_values[i - 1] = lambda * log_split + (1.0f - lambda) * uniform_split;
    }
}

void CSM::computeLightProjection(int i, float aspect, glm::mat4 &view_inverse)
{
    float minz, maxz, minx, maxx, miny, maxy, z_near, z_far, z_e;
    glm::vec4 light_space_pos;

    z_near = i < 1 ? 0.1f : z_values[i - 1];
    z_far = z_values[i];

    glm::mat4 projection_inverse = glm::inverse(glm::perspective(glm::radians(45.0f), aspect, z_near, z_far));

    for (int j = 0; j < 8; j++)
    {
        z_e = normalized_pos[j].z > 0 ? -z_far : -z_near;
        light_space_pos = light_view * view_inverse * projection_inverse * (-z_e * normalized_pos[j]);
        if (j == 0)
        {
            minx = light_space_pos.x;
            miny = light_space_pos.y;
            minz = light_space_pos.z;
            maxx = light_space_pos.x;
            maxy = light_space_pos.y;
            maxz = light_space_pos.z;
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
    // std::cout << " minx: " << minx
    //           << " miny: " << miny
    //           << " minz: " << minz
    //           << " maxx: " << maxx
    //           << " maxy: " << maxy
    //           << " maxz: " << maxz << std::endl;
    light_projection_view[i] = glm::ortho(minx, maxx, miny, maxy, -maxz - 10, -minz) * light_view;
    // light_projection_view[i] = glm::ortho(0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 100.0f) * light_view;
}

void CSM::draw()
{
    float aspect = HEIGHT == 0 ? 0 : (float)WIDTH / HEIGHT;
    glm::mat4 view_inverse = glm::inverse(camera.getViewMatrix());

    light_view = glm::lookAt(-(light_direction * glm::vec3(10.0f)), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    updateSplitZ();

    glViewport(0, 0, 800, 800);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    shadow_shader.useProgram();
    glCullFace(GL_FRONT);
    for (int i = 0; i < 3; i++)
    {
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture_array, 0, i);
        glClear(GL_DEPTH_BUFFER_BIT);

        computeLightProjection(i, aspect, view_inverse);
        shadow_shader.setUniformMatrix4("projection", projection);
        shadow_shader.setUniformMatrix4("projection_view", light_projection_view[i]);
        drawScence(shadow_shader);
        /* code */
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, WIDTH, HEIGHT);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    shader.useProgram();
    glCullFace(GL_BACK);
    shader.setUniformMatrix4("projection", projection);
    shader.setUniformMatrix4("light_projection_view[0]", light_projection_view[0]);
    shader.setUniformMatrix4("light_projection_view[1]", light_projection_view[1]);
    shader.setUniformMatrix4("light_projection_view[2]", light_projection_view[2]);
    drawScence(shader);

    sw.draw(texture_array, WIDTH, HEIGHT);
}

void CSM::createVAO()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void *)(sizeof(float) * 6));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CSM::drawScence(Shader &shader)
{
    shader.setUniformMatrix4("view", camera.getViewMatrix());
    box.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setUniform1i("material.tex", 0);
    shader.setUniform1f("material.shininess", 32.0f);
    shader.setUniform3fv("view_pos", camera.getPosition());

    shader.setUniform1f("z_values[0]", z_values[0]);
    shader.setUniform1f("z_values[1]", z_values[1]);
    shader.setUniform1f("z_values[2]", z_values[2]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
    shader.setUniform1i("shadow_texel", 1);

    shader.setUniform3fv("light.direction", light_direction);
    shader.setUniform3fv("light.specular", glm::vec3(1.0f) * glm::vec3(0.3f));
    shader.setUniform3fv("light.diffuse", glm::vec3(1.0f) * glm::vec3(1.0f));
    shader.setUniform3fv("light.ambient", glm::vec3(1.0f) * glm::vec3(0.05f));

    shader.setUniform1i("use_uniform", 0);
    shader.setUniform1i("floor", 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 100);

    shader.setUniform1i("use_uniform", 1);
    shader.setUniformMatrix4("model2", glm::mat4(1.0f));
    shader.setUniform1i("floor", 1);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}

void CSM::bindUniform()
{
    GLsizei vec4size = sizeof(glm::vec4);
    glGenBuffers(1, &model_vbo);
    box.bind();
    glBindBuffer(GL_ARRAY_BUFFER, model_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * 100, models, GL_STATIC_DRAW);
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
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CSM::createModels()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            models[i * 10 + j] = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            models[i * 10 + j] = glm::translate(models[i * 10 + j], glm::vec3((i - 5) * 2 + 1, (j - 5) * 2 + 1, 0.0f));
        }
    }
}

void CSM::createFramebuffer()
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1, &texture_array);
    // glGenTextures(1, &shadow_texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT, 800, 800, 3, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    // glBindTexture(GL_TEXTURE_2D, shadow_texture);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 800, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_texture, 0);
    // glBindTexture(GL_TEXTURE_2D, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
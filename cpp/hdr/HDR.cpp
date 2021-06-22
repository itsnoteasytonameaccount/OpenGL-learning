#include "HDR.h"

static int WIDTH = 800, HEIGHT = 600;
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), 0.0f, 90.0f);
const char *const vertex_shader_path = "dist/HDR/shader/hdr.vs";
const char *const fragment_shader_path = "dist/HDR/shader/hdr.fs";
const char *const vertex_shader_path2 = "dist/HDR/shader/hdr_render.vs";
const char *const fragment_shader_path2 = "dist/HDR/shader/hdr_render.fs";
float vertices[] = {
    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 0.0f};

HDR::HDR(/* args */) : box(1),
                       GLWindow(&camera), c((Drawer *)new Adjust(&camera))
{
    initWindow("hdr & 泛光", WIDTH, HEIGHT);
    shader.readFile(vertex_shader_path, fragment_shader_path);
    hdrShader.readFile(vertex_shader_path2, fragment_shader_path2);
    blurShader.readFile(vertex_shader_path2, "dist/HDR/shader/bloom_blur.fs");
    bloomShader.readFile(vertex_shader_path2, "dist/HDR/shader/bloom.fs");
    lightShader.readFile(vertex_shader_path, "dist/resources/shader/direct_output.fs");
    Texture::LoadTexture("dist/blinn_phong/assets/wood.png", &texture);
    Texture::LoadTexture("dist/resources/images/container2.png", &container);
    Texture::LoadTexture("dist/resources/images/container2_specular.png", &container_specular);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    box.createVAO();
    createVAO();
    createFramebuffer();
    c.initImGui(window);
    dw.init();
}

HDR::~HDR()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteTextures(1, &texture);
    glDeleteTextures(1, &container);
    glDeleteTextures(1, &container_specular);
    glDeleteTextures(1, &first_render_texture);
}

void HDR::getKeyInput()
{
    GLWindow::getKeyInput();
    camera.ProcessKeyboardInput(direction, delta);
}

void HDR::draw()
{
    Adjust *adj = (Adjust *)c.getDrawer();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Shader &renderShader = adj->scene ? bloomShader : hdrShader;
    if (adj->scene)
        renderScene1();
    else
        renderScene0();

    blur();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderShader.useProgram();
    renderShader.setUniform1i("mode", adj->mode);
    renderShader.setUniform1f("exposure", adj->exposure);
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, first_render_texture);
    renderShader.setUniform1i("hdrTex", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, buffers[0]);
    renderShader.setUniform1i("blurTex", 1);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    dw.draw(first_render_texture, WIDTH, HEIGHT, (float)WIDTH * 0.3);
    dw.draw(buffers[0], WIDTH, HEIGHT);

    c.draw();
}

void HDR::setLights(glm::vec3 positions[], glm::vec3 colors[], int n)
{
    char position[30] = "lights[0].position";
    char diffuse[30] = "lights[0].diffuse";
    char specular[30] = "lights[0].specular";
    char ambient[30] = "lights[0].ambient";
    for (int i = 0; i < n; i++)
    {
        position[7] = i + 48;
        diffuse[7] = i + 48;
        specular[7] = i + 48;
        ambient[7] = i + 48;
        shader.setUniform3fv(position, glm::vec3(camera.getViewMatrix() * glm::vec4(positions[i], 1.0f)));
        shader.setUniform3fv(diffuse, colors[i]);
        shader.setUniform3fv(specular, colors[i] * 0.3f);
        shader.setUniform3fv(ambient, colors[i] * 0.01f);

        // glm::mat4 model = glm::translate(glm::mat4(1.0f), positions[i]);
        // shader.setUniformMatrix4("model", model);
        // box.bind();
        // box.draw();
    }
}

void HDR::createVAO()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(sizeof(float) * 2));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void HDR::createFramebuffer()
{
    GLuint attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    for (int i = 0; i < 2; i++)
    {
        glGenFramebuffers(1, frames + i);
        glBindFramebuffer(GL_FRAMEBUFFER, frames[i]);

        glGenTextures(1, buffers + i);
        glBindTexture(GL_TEXTURE_2D, buffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffers[i], 0);
    }

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &first_render_texture);
    glBindTexture(GL_TEXTURE_2D, first_render_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, first_render_texture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, buffers[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, buffers[0], 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDrawBuffers(2, attachments);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDR::blur()
{
    for (int i = 0; i < 10; i++)
    {
        int current = (i + 1) % 2;
        glBindFramebuffer(GL_FRAMEBUFFER, frames[current]);
        glClear(GL_COLOR_BUFFER_BIT);
        blurShader.useProgram();
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, buffers[1 - current]);
        blurShader.setUniform1i("horizontal", current);
        blurShader.setUniform1i("tex", 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void HDR::renderScene0()
{
    glm::vec3 positions[] = {glm::vec3(0.0f, 0.0f, 49.5f), glm::vec3(-1.4f, -1.9f, 9.0f), glm::vec3(0.0f, -1.8f, 4.0f), glm::vec3(0.8f, -1.7f, 6.0f)};
    glm::vec3 colors[] = {glm::vec3(200.0f, 200.0f, 200.0f), glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.2f), glm::vec3(0.0f, 0.1f, 0.0f)};

    glm::mat4 view = camera.getViewMatrix();
    float aspect = HEIGHT == 0 ? 0.0f : (float)WIDTH / HEIGHT;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
    glm::mat4 model(1.0f);
    shader.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setUniformMatrix4("view", view);
    shader.setUniformMatrix4("projection", projection);
    shader.setUniformMatrix3("normalMatrix", glm::inverse(glm::transpose(glm::mat3(view * model))));
    shader.setUniform1i("material.tex", 0);
    shader.setUniform1f("material.shininess", 32.0f);
    shader.setUniform1i("scene", 0);
    shader.setUniform1i("normalFlip", 1);
    setLights(positions, colors);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
    model = glm::scale(model, glm::vec3(5.0f, 5.0f, 55.0f));
    shader.setUniformMatrix4("model", model);
    box.bind();
    box.draw();
}

void HDR::renderScene1()
{
    glm::mat4 model(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    float aspect = HEIGHT == 0 ? 0.0f : (float)WIDTH / HEIGHT;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    std::vector<glm::vec3> lightPositions;
    lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
    lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
    lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
    lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
    // colors
    std::vector<glm::vec3> lightColors;
    lightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
    lightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
    lightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
    lightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));

    shader.useProgram();

    shader.setUniformMatrix4("view", view);
    shader.setUniformMatrix4("projection", projection);
    shader.setUniform1i("material.tex", 0);
    shader.setUniform1f("material.shininess", 32.0f);
    shader.setUniform1i("scene", 1);

    setLights(&lightPositions[0], &lightColors[0]);
    box.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader.setUniform1i("material.specularTex", 0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
    shader.setUniformMatrix3("normalMatrix", glm::inverse(glm::transpose(glm::mat3(view * model))));
    shader.setUniformMatrix4("model", model);
    box.draw();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, container_specular);
    shader.setUniform1i("material.specularTex", 1);
    shader.setUniform1i("normalFlip", 0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, 2.0f * glm::vec3(0.5f));
    shader.setUniformMatrix3("normalMatrix", glm::inverse(glm::transpose(glm::mat3(view * model))));
    shader.setUniformMatrix4("model", model);
    box.draw();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    shader.setUniformMatrix3("normalMatrix", glm::inverse(glm::transpose(glm::mat3(view * model))));
    shader.setUniformMatrix4("model", model);
    box.draw();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
    model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, 2.0f * glm::vec3(1.25));
    shader.setUniformMatrix3("normalMatrix", glm::inverse(glm::transpose(glm::mat3(view * model))));
    shader.setUniformMatrix4("model", model);
    box.draw();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
    model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    shader.setUniformMatrix3("normalMatrix", glm::inverse(glm::transpose(glm::mat3(view * model))));
    shader.setUniformMatrix4("model", model);
    box.draw();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
    model = glm::scale(model, 2.0f * glm::vec3(0.5f));
    shader.setUniformMatrix3("normalMatrix", glm::inverse(glm::transpose(glm::mat3(view * model))));
    shader.setUniformMatrix4("model", model);
    box.draw();

    lightShader.useProgram();
    lightShader.setUniformMatrix4("projection", projection);
    lightShader.setUniformMatrix4("view", view);
    box.bind();
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(lightPositions[i]));
        model = glm::scale(model, 2.0f * glm::vec3(0.25f));
        lightShader.setUniformMatrix4("model", model);
        lightShader.setUniform3fv("color", lightColors[i]);
        box.draw();
    }
}
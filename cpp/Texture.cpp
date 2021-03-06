#include "Texture.h"
// Texture::Texture(const char *path)
// {
//     status = loadTexture(path);
// }
Texture::Texture()
{
}

Texture::~Texture()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &texture1);
}
int Texture::loadTexture(const char *path, int id, unsigned int type, unsigned int *texture)
{
    glGenTextures(id, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load(path, &width, &height, &nChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "load image failed" << std::endl;
        return LOAD_IMAGE_FAILED;
    }
    stbi_image_free(data);
    return 0;
}
int Texture::loadTexture(const char *path, int id)
{
    return loadTexture(path, id, GL_RGB, &texture1);
}

int Texture::createVAO()
{
    float vertices[] = {
        // ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
    };
    unsigned int indices[] = {0, 1, 3, 1, 2, 3};
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return 0;
}

int Texture::createVAO(int flag)
{
    return 0;
}

// void Texture::draw()
// {
//     unsigned int mytransform;
//     float scale = glm::sin((float)glfwGetTime());
//     scale = (scale + 1) / 2;
//     glm::mat4 trans = glm::mat4(1.0f);
//     trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
//     trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
//     mytransform = glGetUniformLocation(shader->program, "mytransform");
//     glClearColor(0.5, 0.5, 0.2, 0.3);
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     shader->useProgram();
//     glUniformMatrix4fv(mytransform, 1, GL_FALSE, glm::value_ptr(trans));
//     glBindVertexArray(VAO);
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

//     trans = glm::mat4(1.0f);
//     trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
//     trans = glm::scale(trans, glm::vec3(scale, scale, 1));
//     glUniformMatrix4fv(mytransform, 1, GL_FALSE, glm::value_ptr(trans));
//     glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//     // glDrawArrays(GL_TRIANGLES, 0, 4);
// }
void Texture::draw()
{
    // unsigned int mytransform;
    unsigned int projection, model, view;
    glm::mat4 projection_mat = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 model_mat = glm::mat4(1.0f);
    glm::mat4 view_mat = glm::mat4(1.0f);
    model_mat = glm::rotate(model_mat, glm::radians(-55.0f), glm::vec3(1.0f, 0, 0));
    view_mat = glm::translate(view_mat, glm::vec3(0, 0, -3.0f));
    // float scale = glm::sin((float)glfwGetTime());
    // scale = (scale + 1) / 2;
    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    // trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // mytransform = glGetUniformLocation(shader->program, "mytransform");

    glClearColor(0.5, 0.5, 0.2, 0.3);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->useProgram();
    projection = glGetUniformLocation(shader->program, "projection");
    view = glGetUniformLocation(shader->program, "view");
    model = glGetUniformLocation(shader->program, "model");
    // glUniformMatrix4fv(mytransform, 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(view_mat));
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projection_mat));
    glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(model_mat));
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // trans = glm::mat4(1.0f);
    // trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
    // trans = glm::scale(trans, glm::vec3(scale, scale, 1));
    // glUniformMatrix4fv(mytransform, 1, GL_FALSE, glm::value_ptr(trans));
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glDrawArrays(GL_TRIANGLES, 0, 4);
}

// int getTexture()
// {
//     return texture;
// }

// int main()
// {
//     Texture texture;
//     unsigned int texture2;
//     texture.initWindow(500, 500, "hello world");
//     texture.shader = new Shader("./dist/shader/textureVShader.vs", "./dist/shader/textureFShader.fs");
//     texture.loadTexture("c/container.jpg", 1, GL_RGB, &texture.texture1);
//     stbi_set_flip_vertically_on_load(true);
//     texture.loadTexture("c/awesomeface.png", 1, GL_RGBA, &texture.texture2);
//     stbi_set_flip_vertically_on_load(false);
//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, texture.texture1);
//     glActiveTexture(GL_TEXTURE1);
//     glBindTexture(GL_TEXTURE_2D, texture.texture2);
//     texture.shader->useProgram();
//     glUniform1i(glGetUniformLocation(texture.shader->program, "ourTexture"), 0);
//     glUniform1i(glGetUniformLocation(texture.shader->program, "ourTexture2"), 1);
//     texture.createVAO();
//     texture.mainLoop();
//     return 0;
// }
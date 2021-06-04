#include "NormalMapping.h"

static int WIDTH = 800, HEIGHT = 600;
static float rotate = 0.0f;
static Camera camera;
// static const float vertices[] = {
//     -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//     1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//     -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
//     1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
//     1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
//     -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f};
const char *const vertex_shader_path = "dist/normal_mapping/shader/normal_mapping.vs";
const char *const fragment_shader_path = "dist/normal_mapping/shader/normal_mapping.fs";

NormalMapping::NormalMapping(/* args */) : GLWindow(), c(new Adjust())
{
    initWindow("法线贴图", WIDTH, HEIGHT);
    shader.readFile(vertex_shader_path, fragment_shader_path);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    Texture::LoadTexture("dist/normal_mapping/images/brickwall.jpg", &texture);
    // Texture::LoadTexture("dist/normal_mapping/images/normal_mapping_normal_map.png", &normal_map);
    Texture::LoadTexture("dist/normal_mapping/images/brickwall_normal.jpg", &normal_map);

    m = new Model((char *)"dist/normal_mapping/cyborg/cyborg.obj");
    // m = new Model((char *)"dist/cubemaps/nanosuit/nanosuit.obj");
    createVAO();
    c.initImGui(window);
}

NormalMapping::~NormalMapping()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, &texture);
    delete m;
}

void NormalMapping::createVAO()
{
    // positions
    glm::vec3 pos1(-1.0, 1.0, 0.0);
    glm::vec3 pos2(-1.0, -1.0, 0.0);
    glm::vec3 pos3(1.0, -1.0, 0.0);
    glm::vec3 pos4(1.0, 1.0, 0.0);
    // texture coordinates
    glm::vec2 uv1(0.0, 1.0);
    glm::vec2 uv2(0.0, 0.0);
    glm::vec2 uv3(1.0, 0.0);
    glm::vec2 uv4(1.0, 1.0);
    // normal vector
    glm::vec3 nm(0.0, 0.0, 1.0);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    // - triangle 1
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent1 = glm::normalize(tangent1);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent1 = glm::normalize(bitangent1);

    // - triangle 2
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
    tangent2 = glm::normalize(tangent2);

    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
    bitangent2 = glm::normalize(bitangent2);
    GLfloat quadVertices[] = {
        // Positions            // normal         // TexCoords  // Tangent                          // Bitangent
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z};

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glm::vec3 TBN = glm::mat3(glm::vec3(1.0f, 2.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f)) * glm::vec3(1.0f, 1.0f, 1.0f);
    std::cout << TBN.x << TBN.y << TBN.z << std::endl;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid *)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid *)(11 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void NormalMapping::draw()
{
    glm::mat4 view = camera.getViewMatrix();
    float aspect = HEIGHT > 0 ? (float)WIDTH / HEIGHT : 0;
    Adjust *adjust = (Adjust *)c.drawer;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 50.0f);
    glm::mat4 model(1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 1.0f, 0.0f));

    shader.useProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal_map);
    shader.setUniformMatrix4("projection", projection);
    shader.setUniformMatrix4("model", model);
    shader.setUniformMatrix4("view", view);
    shader.setUniformMatrix3("normal_matrix", glm::inverse(glm::transpose(glm::mat3(view * model))));
    shader.setUniform3fv("light.position", glm::vec3(view * glm::vec4(adjust->light->position, 1.0f)));
    shader.setUniform3fv("light.diffuse", glm::vec3(1.0f) * adjust->light->diff);
    shader.setUniform3fv("light.ambient", glm::vec3(1.0f) * adjust->light->ambient);
    shader.setUniform3fv("light.specular", glm::vec3(1.0f) * adjust->light->spec);
    shader.setUniform1i("material.texture_diffuse1", 0);
    shader.setUniform1i("material.texture_specular1", 0);
    shader.setUniform1i("material.texture_normal", 1);
    shader.setUniform1f("material.shininess", adjust->shininess);
    shader.setUniform1i("use_normal_map", adjust->mode);

    if (adjust->object == 0)
    {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    else
    {
        m->draw(shader);
    }
    c.draw();
}

void NormalMapping::getKeyInput()
{
    GLWindow::getKeyInput();
    if (getKeyPress(GLFW_KEY_D))
    {
        rotate += 40.0f * delta;
    }
    if (getKeyPress(GLFW_KEY_A))
    {
        rotate -= 40.f * delta;
    }
    camera.ProcessKeyboardInput(direction, delta);
}
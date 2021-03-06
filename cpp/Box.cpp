#include "Box.h"
static Camera *camera = NULL;
static void _mouse_callback(GLFWwindow *window, double xpos, double ypos);
static void _scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
static float lastX = 400;
static float lastY = 300;
static float firstMouse = true;
void Box::createVAO(unsigned int texture1, unsigned int texture2)
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
    shader->useProgram();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUniform1i(glGetUniformLocation(shader->program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader->program, "texture2"), 1);
}
void Box::draw()
{
    unsigned int model, view, projection;
    float width = getWidth(), height = getHeight();
    glm::mat4 modelm = glm::mat4(1.0f),
              projectionm = glm::perspective(glm::radians(50.0f), (float)width / (float)height, 0.1f, 100.0f),
              viewm = camera->GetViewMatrix();
    glClearColor(0, 0.5f, 0.5f, 0.2);
    modelm = glm::rotate(modelm, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0));
    modelm = glm::rotate(modelm, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader->useProgram();

    model = glGetUniformLocation(shader->program, "model");
    projection = glGetUniformLocation(shader->program, "projection");
    view = glGetUniformLocation(shader->program, "view");
    glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(projectionm));
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(viewm));

    glBindVertexArray(VAO);
    for (int i = 0; i < 10; i++)
    {
        float angle = 20.0f * i;
        modelm = glm::translate(glm::mat4(1.0f), cubePositions[i]);
        modelm = glm::rotate(modelm, glm::radians(angle) + (float)(i == 1 || i % 3 == 0 ? glfwGetTime() : 0), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(modelm));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Box::initShader()
{
    shader = new Shader("./dist/shader/boxVShader.vs", "./dist/shader/boxFShader.fs");
}

Shader *Box::getShader()
{
    return shader;
}

void Box::getKeyInput()
{
    GLWindow::getKeyInput();
    // std::cout << "delta:" << delta << std::endl;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(RIGHT, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(LEFT, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(BACKWARD, delta);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera->ProcessKeyboard(FORWARD, delta);
    }
}
Box::Box(/* args */)
{
    camera = new Camera();
    mouse_callback = _mouse_callback;
    scroll_callback = _scroll_callback;
}

Box::~Box()
{
    delete camera;
}

void _mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera->ProcessMouseMovement(xoffset, yoffset);
    // std::cout << "mouse move:" << std::endl;
    // camera->ProcessMouseMovement(xpos, ypos);
}
void _scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
}
int main()
{
    Texture texture;
    Box box;
    // Camera *camera = new Camera();
    unsigned int texture1, texture2;
    box.initWindow(800, 600, "box");
    box.initShader();
    glEnable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    texture.loadTexture("c/container.jpg", 1, GL_RGB, &texture1);
    stbi_set_flip_vertically_on_load(true);
    glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, texture2);
    texture.loadTexture("c/awesomeface.png", 1, GL_RGBA, &texture2);
    stbi_set_flip_vertically_on_load(false);
    // box.getShader()->useProgram();
    box.createVAO(texture1, texture2);
    box.mainLoop();
    // std::cout << "now:" << glfwGetTime() << std::endl;
    // for (int i = 0; i < 100000; i++)
    // {
    //     camera->processMouseMovement(0, 0);
    //     camera->getViewMatrix();
    // }
    // std::cout << "end:" << glfwGetTime() << std::endl;
    return 0;
}
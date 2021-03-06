#include "Light.h"

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 vertex;\n"
                                 "layout (location = 1) in vec3 aNormal;\n"
                                 "uniform mat4 view;\n"
                                 "uniform mat4 projection;\n"
                                 "uniform mat4 model;\n"
                                 "out vec3 FragPos;\n"
                                 "out vec3 Normal;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = projection*view*model * vec4(vertex,1);\n"
                                 "FragPos = vec3(model * vec4(vertex, 1.0));\n"
                                 "Normal = aNormal;\n"
                                 "}\n\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "uniform vec3 block_color;\n"
                                   "uniform vec3 light_color;\n"
                                   "uniform vec3 lightPos;\n"
                                   "uniform vec3 viewPos;\n"
                                   "in vec3 FragPos;\n"
                                   "in vec3 Normal;\n"
                                   "out vec4 color;\n"
                                   "void main()\n"
                                   "{\n"
                                   "    float amibient = 0.1f;\n"
                                   "    float specularStrength = 0.5f;\n"
                                   "    vec3 viewDir = normalize(viewPos - FragPos);\n"
                                   "    vec3 norm = normalize(Normal);\n"
                                   "    vec3 lightDir = normalize(lightPos - FragPos);\n"
                                   "    vec3 reflectDir = reflect(-lightDir,norm);\n"
                                   "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
                                   "    vec3 specular = specularStrength * spec * light_color;\n"
                                   "    float diff = max(dot(norm, lightDir), 0.0);\n"
                                   "    vec3 diffuse = diff * light_color;\n"
                                   "    vec3 result = (amibient+ diffuse + specular) *block_color;\n"
                                   "   color = vec4(result,1);\n"
                                   "}\n\0";
const char *lightFragmentShaderSource = "#version 330 core\n"
                                        "out vec4 color;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   color = vec4(1);\n"
                                        "}\n\0";

static int WIDTH = 800;
static int HEIGHT = 600;
static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
const float vertexData[] = {
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, -0.5f, 0.5f,
    -0.5f, -0.5f, -0.5f};
const unsigned int indices[] = {
    0, 2, 6, 0, 4, 6,
    0, 1, 2, 1, 2, 3,
    0, 1, 4, 1, 4, 5,
    4, 6, 7, 4, 7, 5,
    6, 7, 2, 7, 2, 3,
    7, 5, 1, 7, 1, 3};
float vertices[] = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};
const char *viewMatrixUniformName = "view";//"viewMatrix";
const char *modelMatrixUniformName = "model";//"modelMatrix";
const char *projectMatrixUniformName = "projection";//"projectMatrix";

void setViewport(GLFWwindow *window, int width, int height)
{
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

void mouseMove(GLFWwindow *window, double xpos, double ypos)
{
    camera.ProcessMouseMovement(xpos, ypos);
}
void scroll(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

Light::Light()
{
    initWindow();
    lastTime = glfwGetTime();
    vaoid = 1;
    
    object_shader.readFile(vertex_shader_path, fragment_shader_path);
    light_shader.readFile(vertex_shader_path, light_fragment_shader_path);
}
Light::~Light()
{
    glDeleteVertexArrays(vaoid, &vao);
    glDeleteBuffers(vaoid, &vbo);
    glDeleteProgram(program);
    glfwTerminate();
}
void Light::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "light sence", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, setViewport);
    glfwSetCursorPosCallback(window, mouseMove);
    glfwSetScrollCallback(window, scroll);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return;
    }
    setViewport(window, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    std::cout << "window init success" << std::endl;
}
void Light::createVao()
{
    object_shader.useProgram();
    // glUseProgram(program);
    glGenBuffers(vaoid, &vbo);
    // glGenBuffers(vaoid, &ebo);
    glGenVertexArrays(vaoid, &vao);
    glBindVertexArray(vao);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // std::cout << "create vao success" << std::endl;
}

void Light::createLightVao()
{
    light_shader.useProgram();
    // glUseProgram(lightProgram);
    glGenBuffers(2, &light_vbo);
    glGenBuffers(2, &light_ebo);
    glGenVertexArrays(2, &light_vao);
    glBindVertexArray(light_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light_ebo);
    glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // std::cout << "create light_vao success" << std::endl;
}

void Light::compileProgram()
{
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    int vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
    int lightFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    int status;
    char log[512];
    program = glCreateProgram();
    lightProgram = glCreateProgram();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(vertexShader2, 1, &vertexShaderSource, NULL);
    glShaderSource(lightFragmentShader, 1, &lightFragmentShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(vertexShader);
    glCompileShader(vertexShader2);
    glCompileShader(lightFragmentShader);
    glCompileShader(fragmentShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, log);
        std::cout << "compile vertexShader failed:" << log << std::endl;
        return;
    }
    glGetShaderiv(fragmentShader, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, log);
        std::cout << "compile fragmentShader failed:" << log << std::endl;
        return;
    }
    glGetShaderiv(lightFragmentShader, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetShaderInfoLog(lightFragmentShader, 512, NULL, log);
        std::cout << "compile lightFragmentShader failed:" << log << std::endl;
        return;
    }
    glAttachShader(program, vertexShader2);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glAttachShader(lightProgram, vertexShader);
    glAttachShader(lightProgram, lightFragmentShader);
    glLinkProgram(lightProgram);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cout << "link program failed:" << log << std::endl;
        return;
    }
    glGetProgramiv(lightProgram, GL_LINK_STATUS, &status);
    if (!status)
    {
        glGetProgramInfoLog(lightProgram, 512, NULL, log);
        std::cout << "link lightProgram failed:" << log << std::endl;
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(vertexShader2);
    glDeleteShader(fragmentShader);
    glDeleteShader(lightFragmentShader);
    // glUseProgram(program);
    // std::cout << "program compile success" << std::endl;
}
void Light::draw()
{
    glm::vec3 lightColor(0.33f, 0.42f, 0.18f);
    glm::vec3 toyColor(1.0f, 0.5f, 0.31f);
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

    glm::mat4 project = glm::perspective(glm::radians(camera.getZoom()), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -2.0f));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    object_shader.useProgram();
    // glUseProgram(program);
    glBindVertexArray(vao);
    object_shader.setUniformMatrix4("model", model);
    object_shader.setUniformMatrix4("projection", project);
    object_shader.setUniformMatrix4("view", camera.getViewMatrix());
    // object_shader.setUniformMatrix3("normalModel", normal_model);
    object_shader.setUniform3fv("lightPos", lightPos);
    object_shader.setUniform3fv("lightColor", lightColor);
    object_shader.setUniform3fv("objectColor", toyColor);
    object_shader.setUniform3fv("viewPos", camera.getPosition());
    // setUniformMatrix4(viewMatrixUniformName, camera.getViewMatrix(), program);
    // setUniformMatrix4(projectMatrixUniformName, project, program);
    // setUniformMatrix4(modelMatrixUniformName, model, program);
    // glUniform3fv(glGetUniformLocation(program, "block_color"), 1, glm::value_ptr(toyColor));
    // glUniform3fv(glGetUniformLocation(program, "light_color"), 1, glm::value_ptr(lightColor));
    // glUniform3fv(glGetUniformLocation(program, "lightPos"), 1, glm::value_ptr(lightPos));
    // glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, glm::value_ptr(camera.getPosition()));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    light_shader.useProgram();
    // glUseProgram(lightProgram);
    glBindVertexArray(light_vao);
    model = glm::translate(glm::mat4(1.0f), lightPos);
    light_shader.setUniformMatrix4("model", model);
    light_shader.setUniformMatrix4("projection", project);
    light_shader.setUniformMatrix4("view", camera.getViewMatrix());

    // setUniformMatrix4(viewMatrixUniformName, camera.getViewMatrix(), lightProgram);
    // setUniformMatrix4(projectMatrixUniformName, project, lightProgram);
    // setUniformMatrix4(modelMatrixUniformName, model, lightProgram);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    // std::cout << "draw end" << std::endl;
}

void Light::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        float current = (float)glfwGetTime();
        delta = current - lastTime;
        lastTime = current;
        getKeyInput();
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Light::getKeyInput()
{
    int direction = 0x00;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        direction |= _CAMERA_UP;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        direction |= _CAMERA_DOWN;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        direction |= _CAMERA_RIGHT;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        direction |= _CAMERA_LEFT;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        direction |= _CAMERA_FORWARD;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        direction |= _CAMERA_BACKWARD;
    }
    camera.ProcessKeyboardInput(direction, delta);
}
void Light::setUniformMatrix4(const char *name, glm::mat4 value, unsigned int program)
{
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(value));
}
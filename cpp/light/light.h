#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Camera.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <Shader.h>


static const char *const vertex_shader_path = "dist/shader/phong_shading/vertex_shader.vs";
static const char *const fragment_shader_path = "dist/shader/phong_shading/fragment_shader.fs";
static const char *const light_fragment_shader_path = "dist/shader/phong_shading/light_fragment_shader.fs";
class Light
{
private:
    // int width, height;
    unsigned int vao, vaoid, vbo, ebo, program, lightProgram, light_vao, light_vbo, light_ebo;
    float lastTime, delta;
    GLFWwindow *window;
    glm::mat4 view, projection;
    Shader object_shader, light_shader;

public:
    void createVao();
    void createLightVao();
    // void createVAO();
    void initWindow();
    void compileProgram();
    void mainLoop();
    void draw();
    void prepare();
    void setUniformMatrix4(const char *name, glm::mat4 value, unsigned int program);
    void getKeyInput();
    Light();
    ~Light();
};

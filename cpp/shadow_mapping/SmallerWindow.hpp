#include <glad/glad.h>
#include <Shader.h>

#ifndef __SMALLER_WINDOW__
#define __SMALLER_WINDOW__
static char *vertex_shader = "#version 330 core\n"
                            "layout (location = 0) in vec2 position;"
                            "layout (location = 1) in vec2 texCoords;"
                            "out vec2 TexCoords;\n"
                            "void main()"
                            "{"
                            "    gl_Position = vec4(position, 0.0f, 1.0f);"
                            "    TexCoords = texCoords;"
                            "}";
static char *fragment_shader = "#version 330 core\n"
                              "out vec4 FragColor;"
                              "in  vec2 TexCoords;"
                              ""
                              "uniform sampler2D tex;"
                              ""
                              "void main()"
                              "{"
                              "    float depthValue = texture(tex, TexCoords).r;"
                              "    FragColor = vec4(vec3(depthValue), 1.0);"
                              "}";
static float vertices[] = {
    0.6f, 1.0f, -0.1f, -0.1f,
    0.6f, 0.7f, -0.1f, 1.1f,
    1.0f, 1.0f, 1.1f, 0.0f,

    1.0f, 0.7f, 1.1f, 1.1f,
    1.0f, 1.0f, 1.1f, -0.1f,
    0.6f, 0.7f, -0.1f, 1.1f};
class SmallerWindow
{
private:
    Shader shader;
    unsigned int vao, vbo;
    unsigned int vshader, fshader, program;
    /* data */
public:
    SmallerWindow(){};
    ~SmallerWindow()
    {
        glDeleteProgram(program);
        glDeleteShader(fshader);
        glDeleteShader(vshader);
    };
    void init()
    {
        glGenVertexArrays(2, &vao);
        glGenBuffers(2, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        shader.compileShader(vertex_shader, GL_VERTEX_SHADER, &vshader);
        shader.compileShader(fragment_shader, GL_FRAGMENT_SHADER, &fshader);
        shader.linkProgram(vshader, fshader, &program);
    };
    void draw(unsigned int texture)
    {
        glUseProgram(program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glUseProgram(0);
    };
};
#endif
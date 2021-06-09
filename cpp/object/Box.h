#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <Texture.h>

#ifndef __BOX_H__
#define __BOX_H__
class Box
{
private:
    unsigned int id, vao, vbo, texture1, texture2;
    int texture1num, texture2num;
    /* data */
    static float vertices[];

public:
    Box(unsigned int id);
    void draw(GLenum = GL_TRIANGLES);
    void createVAO(bool has_texture = false);
    void bind();
    void bind(Shader &shader, const char *texture_uniform_name, const char *texture_specular_unfiorm_name);
    ~Box();
};

#endif
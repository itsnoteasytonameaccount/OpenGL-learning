#include <Shader.h>
#include <Box.h>
#include <glm/glm.hpp>

#ifndef __DEBUG_WINDOW__
#define __DEBUG_WINDOW__
class DebugWindow
{
private:
    Shader shader;
    Box box;
    /* data */
public:
    DebugWindow() : box(1){};
    ~DebugWindow(){};
    void init()
    {
        const char *const vertex_shader_path = "dist/point_shadows/shader/debug_window.vs";
        const char *const fragment_shader_path = "dist/point_shadows/shader/debug_window.fs";
        shader.readFile(vertex_shader_path, fragment_shader_path);
        box.createVAO();
    };
    void draw(glm::mat4 projection, glm::mat4 view, unsigned int texture, int width, int height, float scale = 0.7f)
    {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        glViewport((int)((1.0f - scale) * (float)width), (int)((1.0f - scale) * (float)height), width, height);
        shader.useProgram();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

        shader.setUniformMatrix4("projection", projection);
        shader.setUniformMatrix4("view", glm::mat3(view));

        box.bind();
        box.draw();
        glUseProgram(0);
        glViewport(0, 0, width, height);

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
    };
};

#endif
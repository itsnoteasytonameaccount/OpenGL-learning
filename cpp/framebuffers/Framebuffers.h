#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Box.h>
#include <Texture.h>
#include "Controller.hpp"

class Framebuffers : public GLWindow
{
private:
    Shader shader, framebuffers_shader;
    Box box;
    Controller c;
    unsigned int texture1, texture2, texture3;
    unsigned int vao, vbo, fbo, rbo, vao1, vbo1, ebo1;

    /* data */
public:
    void draw();
    void getKeyInput();
    void createFloorVAO();
    void createSquareVAO();
    void createFramebuffer();
    Framebuffers(/* args */);
    ~Framebuffers();
};

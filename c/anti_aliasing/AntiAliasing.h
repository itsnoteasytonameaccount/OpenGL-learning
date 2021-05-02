#include <GLWindow.h>
#include <Box.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Texture.h>
#include <time.h>
#include <Controlller.hpp>

class AntiAliasing : public GLWindow
{
private:
    Box box;
    Shader shader, square_shader;
    unsigned int fbo, texture, rbo;
    unsigned int fbo1, texture1, rbo1;
    unsigned int vao, vbo, texture2;
    Controller c;
    /* data */
public:
    void draw();
    void getKeyInput();
    void createMultisampleFramebuffer();
    void createFramebuffer();
    void createUniformBuffer();
    void createVAO();
    AntiAliasing(/* args */);
    ~AntiAliasing();
};

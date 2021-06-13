#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Box.h>

class HDR : public GLWindow
{
private:
    Shader shader;
    Box box;
    unsigned int first_render_texture, vao, vbo, texture;
    /* data */
public:
    void getKeyInput();
    void draw();
    void createVAO();
    HDR(/* args */);
    ~HDR();
};

#include <GLWindow.h>
#include <Box.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Texture.h>

class Advance : public GLWindow
{
private:
    unsigned int texture1, texture2, vao, vbo;
    Box box;
    Shader shader;
    /* data */
public:
    void draw();
    void getKeyInput();
    void createVAO();
    void createUniformBuffer();
    Advance(/* args */);
    ~Advance();
};

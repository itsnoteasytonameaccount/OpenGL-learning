#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Box.h>
#include <Texture.h>

class DepthTest : public GLWindow
{
private:
    Shader shader;
    Box box;
    unsigned int texture1, texture2;
    unsigned int vao, vbo;

    /* data */
public:
    void draw();
    void getKeyInput();
    void createFloorVAO();
    DepthTest(/* args */);
    ~DepthTest();
};

#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Box.h>
#include <Texture.h>
#include <vector>
#include <map>
#include "Controller.hpp"

class Blending : public GLWindow
{
private:
    Shader shader;
    Box box;
    Controller c;
    unsigned int texture1, texture2, texture3, texture4;
    unsigned int vao, vbo, vao1, vbo1, ebo1;

    /* data */
public:
    void draw();
    void getKeyInput();
    void createFloorVAO();
    void createSquareVAO();
    Blending(/* args */);
    ~Blending();
};

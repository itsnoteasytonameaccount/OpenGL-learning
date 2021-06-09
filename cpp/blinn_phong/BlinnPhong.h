#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Texture.h>
#include <Box.h>
#include <Controlller.hpp>
#include "Adjust.h"

class BlinnPhong : public GLWindow
{
private:
    Shader shader, light_shader;
    unsigned int vao, vbo, ebo, texture;
    glm::vec3 light_color;
    Box box;
    Controller c;
    /* data */
public:
    BlinnPhong(/* args */);
    ~BlinnPhong();
    void createVAO();
    void draw();
    void getKeyInput();
};

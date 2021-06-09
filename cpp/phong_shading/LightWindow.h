#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Cube.h>

class LightWindow : public GLWindow
{
private:
    /* data */
    glm::vec3 light_color, object_color, light_pos;
    Shader object_shader, light_shader;
    Cube cube;

public:
    LightWindow();
    ~LightWindow();
    void getKeyInput();
    void draw();
};

#include <Camera.hpp>
#include <Shader.h>
#include <Box.h>
#include <Texture.h>
#include <GLWindow.h>

class LightCasters : public GLWindow
{
private:
    char param[100];
    glm::vec4 light_direction, light_color, light_pos;
    float shininess;
    Shader object_shader, light_shader;
    unsigned int texture0, texture1;
    int texture0num, texture1num;
    Box box;
    static const int SPOT_LIGHT = 1, DIR_LIGHT = 2, POINT_LIGHT = 3;

public:
    void draw();
    void getKeyInput();
    void setLight(char const *name, char index, glm::vec4 vector, glm::vec4 color, int type);
    LightCasters(/* args */);
    ~LightCasters();
};

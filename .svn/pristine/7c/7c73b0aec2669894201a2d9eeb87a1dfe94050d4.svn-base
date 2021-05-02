#include <GLWindow.h>
#include <Shader.h>
#include <Camera.hpp>
#include <Texture.h>
#include <Box.h>

class LightingMaps : public GLWindow
{
private:
    /* data */
    glm::vec3 light_color, specular, light_pos;
    float shininess;
    Shader object_shader, light_shader;
    Box box;
    unsigned int texture0, texture1, texture2;
    int texture0num, texture1num, texture2num;

public:
    LightingMaps(/* args */);
    ~LightingMaps();
    void getKeyInput();
    void draw();
};

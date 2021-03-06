#include <GLWindow.h>
#include "Model.h"
#include <Shader.h>
#include <Box.h>
#include <Controlller.hpp>
#include "IsNormalShow.h"

class ModelLoading : public GLWindow
{
private:
    Shader object_shader, light_shader, normal_shader;
    glm::vec4 light_position, light_direction;
    glm::vec3 light_color;
    Controller c;
    float shininess;
    Model *model;
    Box *box;
    static const int SPOT_LIGHT = 1, DIR_LIGHT = 2, POINT_LIGHT = 3;
    char param[100];
    /* data */
public:
    ModelLoading(/* args */);
    ~ModelLoading();
    void draw();
    void getKeyInput();
    void setLight(char const *name, char index, glm::vec4 vector, glm::vec3 color, int type);
};

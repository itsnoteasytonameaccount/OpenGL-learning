#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Drawer.h>
#include <imgui.h>
#include <iostream>

typedef struct
{
    glm::vec3 position;

    float diff;
    float spec;
    float ambient;
} Light;

class Adjust : public Drawer
{
public:
    glm::vec3 camera_pos;
    float shininess, height_scale;
    int mode, object, divide, use_disp_map;
    Light *light;
    bool show;

    Adjust();
    int draw();
    ~Adjust();
};

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Drawer.h>
#include <imgui.h>
#include <iostream>

typedef struct
{
    glm::vec3 direction;

    float diff;
    float spec;
    float ambient;
} Light;

class Adjust : public Drawer
{
public:
    int show_shadow_map_layer;
    int map_width, map_height, split_quantity;
    glm::vec3 camera_pos;
    float shininess, scale, light_frustum_stretch, lambda;
    Light light;
    bool show;

    Adjust(glm::vec3 light_direction = glm::vec3(0.8f, -0.4f, -1.2f));
    int draw();
    ~Adjust();
};

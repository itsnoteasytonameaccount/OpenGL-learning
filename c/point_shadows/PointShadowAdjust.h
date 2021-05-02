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
    float shininess, degree, near, far, scale;
    Light *light;
    bool show;
    static char *box_options[];

    Adjust();
    int draw();
    ~Adjust();
};

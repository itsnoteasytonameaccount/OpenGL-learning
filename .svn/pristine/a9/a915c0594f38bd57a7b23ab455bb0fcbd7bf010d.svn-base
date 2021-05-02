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
    int box_index;
    glm::vec3 camera_pos;
    glm::vec3 box_pos[3];
    float shininess, degree, near, far, distance;
    Light *light;
    bool show;
    static char *box_options[];

    Adjust();
    int draw();
    ~Adjust();
};

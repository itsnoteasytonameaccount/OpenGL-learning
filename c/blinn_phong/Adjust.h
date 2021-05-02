#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Drawer.h>
#include <imgui.h>

typedef struct
{
    glm::vec3 light_pos;

    float diff;
    float spec;
    float ambient;
} Light;

class Adjust : public Drawer
{
public:
    int mode, light_index, quadratic;
    glm::vec3 camera_pos;
    float shininess, degree, gamma;
    Light *lights;
    bool show;
    static char *light_options[];

    Adjust();
    int draw();
    ~Adjust();
};

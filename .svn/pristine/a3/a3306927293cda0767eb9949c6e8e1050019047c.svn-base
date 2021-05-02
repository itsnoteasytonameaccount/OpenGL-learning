#include <Camera.hpp>
#include <Cube.h>
#include <Shader.h>
#include <GLWindow.h>

typedef struct
{
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
} Light;

typedef struct
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shiness;
} Material;

class MaterialWindow : public GLWindow
{
private:
    /* data */
    glm::vec3 light_color, ambient, specular, diffuse, light_pos;
    float shininess;
    Shader object_shader, light_shader;
    Cube cube;
    // Material material;
    // Light light;

public:
    MaterialWindow();
    ~MaterialWindow();
    void getKeyInput();
    void draw();
};

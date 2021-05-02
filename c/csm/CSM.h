#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Box.h>
#include <Texture.h>

class CSM : public GLWindow
{
private:
    Shader shader, shadow_shader;
    Box box;
    unsigned int fbo, texture_array, model_vbo, texture, vao, vbo;
    glm::mat4 light_projection_view[3];
    float z_values[3];
    glm::mat4 models[100];
    glm::vec3 light_direction;
    glm::vec3 light_view;
    /* data */
public:
    // 默认分成3份
    void updateSplitZ(float = 3.0f);
    // 计算某个阴影贴图的投影矩阵
    void computeLightProjection(int i, float aspect, glm::mat4 &view_inverse);
    void createFramebuffer();
    void bindUniform();
    void createModels();
    void createVAO();
    void draw();
    CSM(/* args */);
    ~CSM();
};

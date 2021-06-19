#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Box.h>
#include <Texture.h>
#include <math.h>
#include <Controlller.hpp>
#include "SmallerWindow.hpp"
#include "CSMAdjust.h"
#define SPLIT_QUANTITY 3

class CSM : public GLWindow
{
private:
    Shader shader, shadow_shader;
    Box box;
    unsigned int fbo, texture_array, model_vbo, texture, vao, vbo, ebo;
    glm::mat4 light_projection_view[SPLIT_QUANTITY];
    float z_values[SPLIT_QUANTITY];
    glm::mat4 models[100];
    glm::mat4 light_view;
    SmallerWindow sw;
    Adjust adj;
    Controller *con;
    /* data */
public:
    // 默认分成3份
    void updateSplitZ(int = SPLIT_QUANTITY);
    // 计算某个阴影贴图的投影矩阵
    void computeLightProjection(int i, float aspect, glm::mat4 &view_inverse);
    void createFramebuffer();
    void bindUniform();
    void createModels();
    void createVAO();
    void draw();
    void getKeyInput();
    void drawScene(Shader &shader);
    CSM(/* args */);
    ~CSM();
};

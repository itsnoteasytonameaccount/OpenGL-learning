#include <GLWindow.h>
#include <Shader.h>
#include <Controlller.hpp>
#include <Camera.hpp>
#include <Texture.h>
#include <Box.h>
#include "SmallerWindow.hpp"
#include "ShadowMappingAdjust.h"

class ShadowMapping : public GLWindow
{
private:
    unsigned int shadow_texel, fbo, vao, vbo, ebo, texture;
    Shader shader, shadow_shader;
    glm::vec3 light_color, light_direction;
    Box box;
    Controller c;
    SmallerWindow sm;
    /* data */
public:
    ShadowMapping(/* args */);
    ~ShadowMapping();
    void createShadowFramebuffer();
    void createVAO();
    void draw();
    void getKeyInput();
};
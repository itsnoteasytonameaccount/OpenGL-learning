#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Box.h>
#include <Controlller.hpp>
#include <vector>
#include "HDRAdjust.h"
#include "DebugWindow.hpp"

class HDR : public GLWindow
{
private:
    Shader shader, hdrShader, blurShader, bloomShader, lightShader;
    Box box;
    Controller c;
    unsigned int first_render_texture, rbo, vao, vbo, texture, fbo, container, container_specular;
    unsigned int frames[2], buffers[2];
    DebugWindow dw;
    /* data */
public:
    void getKeyInput();
    void draw();
    void setLights(glm::vec3 positions[], glm::vec3 colors[], int n = 4);
    void createVAO();
    void createFramebuffer();
    void blur();
    void renderScene0();
    void renderScene1();
    HDR(/* args */);
    ~HDR();
};

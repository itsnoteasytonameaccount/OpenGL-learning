#include <GLWindow.h>
#include <Shader.h>
#include <Controlller.hpp>
#include <Box.h>
#include <Camera.hpp>
#include <Texture.h>
#include "PointShadowAdjust.h"
#include "DebugWindow.hpp"

class PointShadows : public GLWindow
{
private:
    unsigned int texture, depth_texture, fbo;
    Shader shader, shadow_shader;
    Box box;
    /* data */
    Controller c;
    DebugWindow dw;

public:
    PointShadows(/* args */);
    ~PointShadows();
    void createFramebuffer();
    void prepare();
    void renderScene(Shader &shader);
    void draw();
    void getKeyInput();
};

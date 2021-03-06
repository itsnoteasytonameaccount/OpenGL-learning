#include <GLWindow.h>
#include <Shader.h>
#include <Model.hpp>
#include <Texture.h>
#include <Camera.hpp>
#include <Controlller.hpp>
#include "NormalMappingAdjust.h"

class NormalMapping : public GLWindow
{
private:
    Shader shader;
    Model *m;
    unsigned int vbo, vao, normal_map, texture, texture2, normal_map2, disp_map2,texture3, normal_map3, disp_map3;
    Controller c;
    /* data */
public:
    NormalMapping(/* args */);
    ~NormalMapping();
    void createVAO();
    void getKeyInput();
    void draw();
};

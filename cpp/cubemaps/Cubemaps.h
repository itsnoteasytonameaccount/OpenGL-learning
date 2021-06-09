#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Box.h>
#include <Texture.h>
#include "Model.h"
#include "Controller.hpp"

class Cubemaps : public GLWindow
{
private:
    Shader shader, cubemaps_shader;
    Box box;
    unsigned int texture1, texture2, texture3;
    Model *m;
    Controller c;

    /* data */
public:
    void draw();
    void getKeyInput();
    Cubemaps(/* args */);
    ~Cubemaps();
};

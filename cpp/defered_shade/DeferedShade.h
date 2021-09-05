#include <GLWindow.h>
#include <Camera.hpp>
#include <Shader.h>

class DeferedShade : public GLWindow
{
private:
    /* data */
    unsigned int gBuffer, rbo;
    unsigned int textures[3];
    Shader shader;
public:
    virtual void draw();
    void createFramebuffer();
    void renderScene();
    DeferedShade(/* args */);
    ~DeferedShade();
};

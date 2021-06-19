#include <Drawer.h>
#include <imgui.h>
#include <iostream>
#include <Camera.hpp>

class Adjust : public Drawer
{
private:

public:
    float exposure;
    int mode;
    int scene;
    int lastScene;
    Camera *camera;
    Adjust(Camera *camera);
    int draw();
};

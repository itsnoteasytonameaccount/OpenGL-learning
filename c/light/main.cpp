#include "light.h"

int main()
{
    Light light;
    // light.compileProgram();
    light.createVao();
    light.createLightVao();
    light.mainLoop();
    return 0;
}
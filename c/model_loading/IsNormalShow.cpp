#include "IsNormalShow.h"

IsNormalShow::IsNormalShow(/* args */)
{
    mode = 1;
}

IsNormalShow::~IsNormalShow()
{
}

int IsNormalShow::draw()
{
    ImGui::Begin("effect", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::RadioButton("show normal", &mode, 1);
    ImGui::RadioButton("object blast", &mode, 2);
    ImGui::RadioButton("no effect", &mode, 3);

    ImGui::End();
    return 0;
}
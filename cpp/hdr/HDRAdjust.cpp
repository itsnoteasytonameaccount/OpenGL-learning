#include "HDRAdjust.h"

static const char *targets[] = {"with no tone mapping", "Reinhard", "exposure tone mapping"};
static const char *scenes[] = {"scene 0", "scene 1"};

int Adjust::draw()
{
    ImGui::Begin("param adjust");
    if (ImGui::BeginCombo("mode", targets[mode]))
    {
        for (int i = 0; i < 3; i++)
        {
            if (ImGui::Selectable(targets[i], mode == i))
                mode = i;
        }
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("scene", scenes[scene]))
    {
        for (int i = 0; i < 2; i++)
        {
            if (ImGui::Selectable(scenes[i], scene == i))
                scene = i;
        }
        ImGui::EndCombo();
    }
    if (mode == 2)
    {
        ImGui::DragFloat("exposure adjust", &exposure, 0.01f);
    }
    if (lastScene != scene)
    {
        if (scene == 1)
            camera->setYaw(-90.0f);
        if (scene == 0)
            camera->setYaw(90.0f);
    }
    lastScene = scene;
    ImGui::End();
    return 0;
}

Adjust::Adjust(Camera *camera)
{
    this->camera = camera;
    mode = 0;
    exposure = 5.0f;
    scene = 0;
}
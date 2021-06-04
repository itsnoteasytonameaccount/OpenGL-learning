#include "NormalMappingAdjust.h"

Adjust::Adjust()
{
    light = new Light;
    // memset(light, 0, sizeof(Light));
    light->position = glm::vec3(0.0f);
    light->ambient = 0.05f;
    light->diff = 1.0f;
    light->spec = 0.3f;
    mode = 0;
    object = 0;
    shininess = 32.0f;
    show = true;
}

Adjust::~Adjust()
{
    delete light;
}

int Adjust::draw()
{
    ImGui::Begin("param adjust", &show);

    ImGui::BeginGroup();
    ImGui::Text("light adjust");
    ImGui::DragFloat3("light_pos", glm::value_ptr(light->position), 0.1f, 0.0f, 0.0f, "%.1f");
    ImGui::DragFloat("shininess", &shininess, 1.0f);
    ImGui::DragFloat("spec", &light->spec, 0.1f);
    ImGui::DragFloat("diff", &light->diff, 0.01f);
    ImGui::DragFloat("ambient", &light->ambient, 0.01f);
    ImGui::EndGroup();

    if (ImGui::BeginCombo("draw target", object == 0 ? "draw brick wall" : "draw model"))
    {
        if (ImGui::Selectable("draw brick wall", object == 0))
            object = 0;
        if (ImGui::Selectable("draw model", object == 1))
            object = 1;
        ImGui::EndCombo();
    }
    if (ImGui::BeginCombo("use normal map", mode == 0 ? "yes" : "no"))
    {
        if (ImGui::Selectable("yes", mode == 0))
            mode = 0;
        if (ImGui::Selectable("no", mode == 1))
            mode = 1;
        ImGui::EndCombo();
    }

    ImGui::BeginGroup();
    ImGui::Text("camera position: (%.2f, %.2f, %.2f)", camera_pos[0], camera_pos[1], camera_pos[2]);
    ImGui::EndGroup();

    ImGui::End();
    return 0;
}
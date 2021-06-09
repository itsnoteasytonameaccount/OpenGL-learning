#include "NormalMappingAdjust.h"

static const char *targets[] = {"draw brick wall", "draw model", "draw brick wall 2", "draw toy box"};

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
    use_disp_map = 0;
    divide = 8;
    shininess = 32.0f;
    show = true;
    height_scale = 1.0f;
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
    ImGui::DragFloat("height_scale", &height_scale, 0.01f);
    ImGui::DragInt("layer divide amount", &divide);
    ImGui::EndGroup();
    int count = sizeof(targets) / sizeof(targets[0]);
    if (ImGui::BeginCombo("draw target", targets[object]))
    {
        for (int i = 0; i < count; i++)
        {
            if (ImGui::Selectable(targets[i], object == i))
                object = i;
        }
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
    if (ImGui::BeginCombo("use disp map", use_disp_map == 0 ? "no" : "yes"))
    {
        if (ImGui::Selectable("no", use_disp_map == 0))
            use_disp_map = 0;
        if (ImGui::Selectable("yes", use_disp_map == 1))
            use_disp_map = 1;
        ImGui::EndCombo();
    }

    ImGui::BeginGroup();
    ImGui::Text("camera position: (%.2f, %.2f, %.2f)", camera_pos[0], camera_pos[1], camera_pos[2]);
    ImGui::EndGroup();

    ImGui::End();
    return 0;
}
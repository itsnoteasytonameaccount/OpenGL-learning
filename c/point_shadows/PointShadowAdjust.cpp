#include "PointShadowAdjust.h"

Adjust::Adjust()
{
    light = new Light;
    // memset(light, 0, sizeof(Light));
    near = 1.0f;
    far = 7.5f;
    light->position = glm::vec3(0.0f);
    light->ambient = 0.05f;
    light->diff = 1.0f;
    light->spec = 0.3f;
    shininess = 32.0f;
    scale = 0.0f;
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

    ImGui::BeginGroup();
    ImGui::DragFloat("near", &near, 0.1f);
    ImGui::DragFloat("far", &far, 0.1f);
    ImGui::DragFloat("scale", &scale, 0.1f);
    ImGui::EndGroup();

    ImGui::BeginGroup();
    ImGui::Text("camera position: (%.2f, %.2f, %.2f)", camera_pos[0], camera_pos[1], camera_pos[2]);
    ImGui::EndGroup();

    ImGui::End();
    return 0;
}
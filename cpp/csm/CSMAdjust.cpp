#include "CSMAdjust.h"

Adjust::Adjust(glm::vec3 light_direction)
{
    // memset(light, 0, sizeof(Light));
    light.direction = light_direction;
    light.ambient = 0.05f;
    light.diff = 1.0f;
    light.spec = 0.3f;
    shininess = 32.0f;
    scale = 0.3f;
    light_frustum_stretch = 10.0f;
    show_shadow_map_layer = 0;
    lambda = 0.5;
    show = true;
}

Adjust::~Adjust()
{
}

int Adjust::draw()
{
    ImGui::Begin("param adjust", &show);

    ImGui::BeginGroup();
    ImGui::Text("light adjust");
    ImGui::DragFloat3("light_pos", glm::value_ptr(light.direction), 0.1f, 0.0f, 0.0f, "%.1f");
    ImGui::DragFloat("shininess", &shininess, 1.0f);
    ImGui::DragFloat("spec", &light.spec, 0.1f);
    ImGui::DragFloat("diff", &light.diff, 0.01f);
    ImGui::DragFloat("ambient", &light.ambient, 0.01f);
    ImGui::EndGroup();

    ImGui::BeginGroup();
    ImGui::DragFloat("lambda", &lambda, 0.01f);
    ImGui::DragFloat("scale", &scale, 0.01f);
    ImGui::DragFloat("light frustum stretch", &light_frustum_stretch, 0.1f);
    ImGui::EndGroup();

    ImGui::BeginGroup();
    ImGui::Text("displaying shadow map layer");
    ImGui::RadioButton("layer 0", &show_shadow_map_layer, 0);
    ImGui::RadioButton("layer 1", &show_shadow_map_layer, 1);
    ImGui::RadioButton("layer 2", &show_shadow_map_layer, 2);
    ImGui::EndGroup();

    ImGui::BeginGroup();
    ImGui::Text("camera position: (%.2f, %.2f, %.2f)", camera_pos[0], camera_pos[1], camera_pos[2]);
    ImGui::Text("map width: %d, map height: %d, split quantity: %d", map_width, map_height, split_quantity);
    ImGui::EndGroup();

    ImGui::End();
    return 0;
}
#include "Adjust.h"

char *Adjust::light_options[] = {"light1", "light2", "light3", "light4"};

Adjust::Adjust()
{
    lights = new Light[4];
    memset(lights, 0, 4 * sizeof(Light));
    light_index = 0;
    degree = 0.0f;
    show = false;
    mode = 0;
    shininess = 16.0f;
    gamma = 2.2;
    quadratic = 1;
}

Adjust::~Adjust()
{
    delete lights;
}

int Adjust::draw()
{
    Light *light = lights + light_index;
    ImGui::Begin("param adjust", &show);
    ImGui::BeginGroup();
    ImGui::Combo("lights", &light_index, light_options, 4);
    ImGui::DragFloat3("light_pos", glm::value_ptr(light->light_pos), 0.1f, 0.0f, 0.0f, "%.1f");
    ImGui::DragFloat("shininess", &shininess, 1.0f);
    ImGui::DragFloat("spec", &light->spec, 0.1f);
    ImGui::DragFloat("diff", &light->diff, 0.01f);
    ImGui::DragFloat("ambient", &light->ambient, 0.01f);
    ImGui::EndGroup();
    ImGui::BeginGroup();
    ImGui::RadioButton("mode 0", &mode, 0);
    ImGui::RadioButton("mode 1", &mode, 1);
    ImGui::EndGroup();
    ImGui::BeginGroup();
    ImGui::RadioButton("quadratic 0", &quadratic, 0);
    ImGui::RadioButton("quadratic 1", &quadratic, 1);
    ImGui::EndGroup();
    ImGui::BeginGroup();
    ImGui::Text("degree: %.2f", degree);
    ImGui::Text("camera position: (%.2f, %.2f, %.2f)", camera_pos[0], camera_pos[1], camera_pos[2]);
    ImGui::EndGroup();
    ImGui::End();
    return 0;
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
enum
{
    DRAW_BOX = 0,
    DRAW_MODEL
};
enum
{
    REFLECT = 0,
    REFRACT,
    REFLECT_TEXTURE
};
class Controller
{
private:
    /* data */
    bool mode_switch_active;

public:
    int mode, mapping;
    float scale[3] = {1.0f, 1.0f, 1.0f};
    float translate[3] = {0.0f, 0.0f, -3.0f};
    // static
    Controller(/* args */)
    {
        mode = 0;
        mapping = 0;
    };
    ~Controller(){

    };
    void initImGui(GLFWwindow *window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    };
    void dataChange()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("mode set", &mode_switch_active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::BeginGroup();
        ImGui::RadioButton("draw box", &mode, DRAW_BOX);
        ImGui::RadioButton("draw model", &mode, DRAW_MODEL);
        ImGui::RadioButton("draw nothing", &mode, -1);
        ImGui::EndGroup();

        ImGui::Separator();

        ImGui::BeginGroup();
        ImGui::RadioButton("reflect", &mapping, REFLECT);
        ImGui::RadioButton("refract", &mapping, REFRACT);
        if (mode == DRAW_MODEL)
            ImGui::RadioButton("refract texture", &mapping, REFLECT_TEXTURE);
        ImGui::EndGroup();

        ImGui::Separator();

        if (mode == DRAW_MODEL)
        {
            ImGui::BeginGroup();
            ImGui::DragFloat3("translate", translate, 0.1f, -10.0f, 10.0f, "%.1f");
            ImGui::DragFloat3("scale", scale, 0.01f, 0.0f, 1.0f, "%.2f");
            ImGui::EndGroup();
        }

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };
};

// 没办法这样
// char const *Controller::items[] = {"GL_FRONT", "GL_BACK", "GL_BACK_AND_FRONT"};

#endif
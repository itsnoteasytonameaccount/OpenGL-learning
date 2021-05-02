#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
class Controller
{
private:
    /* data */
    bool effect_switch_active;

public:
    int effect;
    // static
    Controller(/* args */)
    {
        effect = -1;
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
    void effectSwitch()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("effect", &effect_switch_active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::RadioButton("lines", &effect, -2);
        ImGui::RadioButton("off", &effect, -1);
        ImGui::RadioButton("inversion", &effect, 0);
        ImGui::RadioButton("grayscale", &effect, 1);
        ImGui::RadioButton("blur", &effect, 2);
        ImGui::RadioButton("sharpen", &effect, 3);
        ImGui::RadioButton("edge detection", &effect, 4);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };
};

// 没办法这样
// char const *Controller::items[] = {"GL_FRONT", "GL_BACK", "GL_BACK_AND_FRONT"};

#endif
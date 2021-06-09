#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <Drawer.h>
#include <EffectRadioButton.hpp>

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
class Controller
{
public:
    Drawer *drawer;
    // static
    Controller(Drawer *drawer = NULL)
    {
        this->drawer = drawer;
    };
    Controller(char **labels, int len, char *title, int *v = NULL)
    {
        this->drawer = new EffectRadioButton(labels, len, title, v);
    };
    ~Controller(){};

    Drawer *getDrawer()
    {
        return drawer;
    };

    void initImGui(GLFWwindow *window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    };
    void draw()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        drawer != NULL && drawer->draw();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };
};

#endif
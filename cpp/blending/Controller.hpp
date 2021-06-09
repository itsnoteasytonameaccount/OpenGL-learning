#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
static const char *const items1[] = {"GL_FRONT", "GL_BACK", "GL_BACK_AND_FRONT"};
static const char *const items2[] = {"GL_CCW", "GL_CW"};
class Controller
{
private:
    /* data */
    bool function_switch_active;

public:
    bool depth_test, stencil_test, blend, cull_face;
    int cull_face_option, front_face_option;
    // static
    Controller(/* args */)
    {
        depth_test = true;
        blend = true;
        stencil_test = true;
        function_switch_active = true;
        front_face_option = 0;
        cull_face_option = 0;
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
    void functionSwitch()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("function controller", &function_switch_active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Checkbox("depth test", &depth_test);
        ImGui::Checkbox("stencil test", &stencil_test);
        ImGui::Checkbox("blend", &blend);
        ImGui::Checkbox("cull face", &cull_face);
        ImGui::End();

        if (cull_face)
        {
            ImGui::Begin("cull face option", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Combo("cull face", &cull_face_option, items1, 3);
            ImGui::Combo("face front", &front_face_option, items2, 2);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    };
    void checkCullOptions()
    {
        if (cull_face)
        {
            switch (cull_face_option)
            {
            case 0:
                glCullFace(GL_FRONT);
                break;
            case 1:
                glCullFace(GL_BACK);
                break;
            case 2:
                glCullFace(GL_FRONT_AND_BACK);
                break;
            }
            switch (front_face_option)
            {
            case 0:
                glFrontFace(GL_CCW);
                break;
            case 1:
                glFrontFace(GL_CW);
                break;
            }
        }
    }
    void checkFlags()
    {
        if (!depth_test)
        {
            glDisable(GL_DEPTH_TEST);
        }
        else
        {
            glEnable(GL_DEPTH_TEST);
        }

        if (!stencil_test)
        {
            glDisable(GL_STENCIL_TEST);
        }
        else
        {
            glEnable(GL_STENCIL_TEST);
        }

        if (!blend)
        {
            glDisable(GL_BLEND);
        }
        else
        {
            glEnable(GL_BLEND);
        }

        if (!cull_face)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
        }
    };
};

// 没办法这样
// char const *Controller::items[] = {"GL_FRONT", "GL_BACK", "GL_BACK_AND_FRONT"};

#endif
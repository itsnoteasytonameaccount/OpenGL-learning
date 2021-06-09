#include "AmountCon.h"
#include <imgui.h>

AmountCon::~AmountCon()
{
}

int AmountCon::draw()
{
    ImGui::Begin("amount control");
    ImGui::BeginGroup();
    ImGui::RadioButton("example 1", &example, 0);
    ImGui::RadioButton("example 2", &example, 1);
    ImGui::RadioButton("example 3", &example, 2);
    ImGui::EndGroup();
    ImGui::DragInt("amount", &amount, 2.0f, 0, 100000);
    ImGui::End();
    return 0;
}

AmountCon::AmountCon(int amount, int example)
{
    this->amount = amount;
    this->example = example;
}
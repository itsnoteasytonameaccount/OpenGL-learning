#include "Drawer.h"
#include <iostream>

class EffectRadioButton : public Drawer
{
private:
    /* data */
    char **labels;
    char *title;
    int *v;
    int len;

public:
    int mode;
    EffectRadioButton(char **labels, int len, char *title, int *v = NULL)
    {
        this->labels = labels;
        this->v = v;
        this->len = len;
        this->title = title;
        mode = v == NULL ? 0 : v[0];
    };
    ~EffectRadioButton(){};
    int draw()
    {
        ImGui::Begin(title);

        for (unsigned i = 0; i < len; i++)
        {
            ImGui::RadioButton(labels[i], &mode, v == NULL ? i : v[i]);
        }

        ImGui::End();
        return 0;
    }
};

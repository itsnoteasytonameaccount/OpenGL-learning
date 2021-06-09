#include <Drawer.h>
#include <imgui.h>

class IsNormalShow : public Drawer
{
private:
    /* data */
public:
    int mode;
    int draw();
    IsNormalShow(/* args */);
    ~IsNormalShow();
};
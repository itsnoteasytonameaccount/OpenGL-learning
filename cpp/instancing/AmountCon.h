#include <Drawer.h>
#include <iostream>

class AmountCon : public Drawer
{
private:
    /* data */

public:
    int example;
    int amount;
    
    int draw();
    AmountCon(int = 1000, int = 0);
    ~AmountCon();
};
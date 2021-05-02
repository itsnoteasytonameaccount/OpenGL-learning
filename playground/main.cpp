#include <iostream>
#include <time.h>

int str_copy(char *dst, char const *src, int start)
{
    char *p1 = dst + start;
    char const *p2 = src;
    while (*p2 != 0)
    {
        *p1++ = *p2++;
    }
    *p1 = 0;

    return p1 - dst;
}

void test_str_copy()
{
    std::cout << "time:" << clock() << std::endl;
    char name[100] = "myname:";
    for (int i = 0; i < 10000000; i++)
    {
        
        // 这样效率高一点
        str_copy(name, "myname:", 0);
        str_copy(name, "ceshiasdasdaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 7);
    }
    std::cout << "time:" << clock() << std::endl;
    std::string name2 = "myname";
    std::string newname;
    std::cout << "time2:" << clock() << std::endl;
    for (int i = 0; i < 10000000; i++)
    {
        name2 + "ceshi";
    }
    std::cout << "time2:" << clock() << std::endl;
}

bool compare(glm)

int main()
{
    test_str_copy();
    int a= 0;
    return 0;
}
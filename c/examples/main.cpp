#include "Camera.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <iomanip>
#include <time.h>

void compare_time()
{
    Camera camera;
    glfwInit();
    std::cout << "time:" << glfwGetTime() << std::endl;
    for (int i = 0; i < 1000000; i++)
    {
        camera.ProcessKeyboard(FORWARD, 1.0f);
        camera.ProcessMouseMovement(0, 0);
        camera.GetViewMatrix();
    }
    std::cout << "time:" << glfwGetTime() << std::endl;
    Camera2 camera2;
    std::cout << "time2:" << glfwGetTime() << std::endl;
    for (int i = 0; i < 1000000; i++)
    {
        camera2.ProcessKeyboard(FORWARD, 1.0f);
        camera2.ProcessMouseMovement(0, 0);
        camera2.GetViewMatrix();
    }
    std::cout << "time2:" << glfwGetTime() << std::endl;
    glfwTerminate();
}

bool compare_matrix(glm::mat4 m1, glm::mat4 m2)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m1[i][j] != m2[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

void print_matrix(glm::mat4 m)
{
    for (int i = 0; i < 4; i++)
    {
        std::cout << " | ";
        for (int j = 0; j < 4; j++)
        {
            std::cout << std::setw(10) << std::left << m[i][j] << ' ';
        }
        std::cout << " | " << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    glm::mat4 m1(1.0f);
    glm::mat4 m2 = glm::scale(m1, glm::vec3(1.1f));
    glm::mat4 m3 = glm::scale(m1, glm::vec3(1.1f)) * m1;
    glm::mat4 m4 = glm::translate(m2, glm::vec3(1.0f));
    glm::mat4 m5 = m2 * glm::translate(m1, glm::vec3(1.0f));
    std::cout << "m1:" << std::endl;
    print_matrix(m1);
    std::cout << "m2:" << std::endl;
    print_matrix(m2);
    std::cout << "m3:" << std::endl;
    print_matrix(m3);
    std::cout << "m4:" << std::endl;
    print_matrix(m4);
    std::cout << "m5:" << std::endl;
    print_matrix(m5);
    return 0;
}
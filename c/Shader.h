#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#ifndef __SHADER_CLASS__
#define __SHADER_CLASS__
class Shader
{
private:
    /* data */

public:
    unsigned int vertexShader, fragmentShader, program;
    const static int SUCCESS = 0;
    const static int READ_FILE_FAILED = 1;
    const static int COMPILE_SHADER_FAILED = 2;
    const static int LINK_PROGRAM_FAILED = 3;
    int status;
    int readFile(const char *vertexPath, const char *fragmentPath);
    int linkProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int *program);
    int compileShader(const char *source, unsigned int type, unsigned int *shader);
    void useProgram();

    Shader(/* args */);
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
};
#endif
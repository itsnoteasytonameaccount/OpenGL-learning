#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#ifndef __SHADER_CLASS__
#define __SHADER_CLASS__
class Shader
{
private:
    unsigned int vertexShader, fragmentShader, geometryShader, program;
    
public:
    const static int SUCCESS = 0;
    const static int READ_FILE_FAILED = 1;
    const static int COMPILE_SHADER_FAILED = 2;
    const static int LINK_PROGRAM_FAILED = 3;
    int status;
    int readFile(const char *vertexPath, const char *fragmentPath, const char *geometryPath = NULL);
    int compileShader(const char *source, unsigned int type, unsigned int *shader);
    int linkProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int *program, unsigned int geometryShader = 0, bool hasGeometryShader = false);
    void useProgram();
    void setUniformMatrix4(const char *name, glm::mat4 value);
    void setUniform3fv(const char *name, glm::vec3 value);
    void setUniform4fv(const char *name, glm::vec4 value);
    void setUniformMatrix3(const char *name, glm::mat3 value);
    void setUniform1f(const char *name, float);
    void setUniform1i(const char *name, int);
    void setUniform3f(const char *name, float, float, float);
    void setUniform4f(const char *name, float, float, float, float);
    void setUniformBlockBinding(const char *name, unsigned int bind_point);

    Shader(/* args */);
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
};
#endif
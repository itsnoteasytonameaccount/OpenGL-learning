#include "Shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    status = readFile(vertexPath, fragmentPath);
}

int Shader::readFile(const char *vertexPath, const char *fragmentPath)
{
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    std::string vertexCode;
    std::string fragmentCode;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vertexFile.rdbuf();
        fShaderStream << fragmentFile.rdbuf();
        vertexFile.close();
        fragmentFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "file read failed" << std::endl;
        return READ_FILE_FAILED;
    }
    if (compileShader(vertexCode.c_str(), GL_VERTEX_SHADER, &vertexShader))
    {
        return COMPILE_SHADER_FAILED;
    }
    if (compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER, &fragmentShader))
    {
        return COMPILE_SHADER_FAILED;
    }
    if (linkProgram(vertexShader, fragmentShader, &program))
    {
        return LINK_PROGRAM_FAILED;
    }
    return 0;
}

int Shader::compileShader(const char *source, unsigned int type, unsigned int *result)
{
    unsigned int shader = glCreateShader(type);
    int success;
    char log[512];
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, log);
        std::cout << "compile " << (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex") << " shader failed:" << std::endl;
        std::cout << log << std::endl;
        return COMPILE_SHADER_FAILED;
    }
    *result = shader;
    return 0;
}
int Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int *result)
{
    unsigned int program = glCreateProgram();
    int success;
    char log[512];
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, log);
        std::cout << "link program failed:\n"
                  << log << std::endl;
        return LINK_PROGRAM_FAILED;
    }
    *result = program;
    return 0;
}

void Shader::useProgram()
{
    glUseProgram(program);
}

Shader::~Shader()
{
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);
}
Shader::Shader()
{
}
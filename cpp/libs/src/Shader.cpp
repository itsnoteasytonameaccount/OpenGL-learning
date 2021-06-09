#include <Shader.h>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    status = readFile(vertexPath, fragmentPath);
}

int Shader::readFile(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
{
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    std::ifstream geometryFile;
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geometryFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vertexFile.open(vertexPath);
        fragmentFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream, gShaderStream;
        vShaderStream << vertexFile.rdbuf();
        fShaderStream << fragmentFile.rdbuf();
        vertexFile.close();
        fragmentFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (geometryPath != NULL)
        {
            geometryFile.open(geometryPath);
            gShaderStream << geometryFile.rdbuf();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure &e)
    {
        std::cout << "file read failed, file path:" << std::endl;
        std::cout << vertexPath << std::endl;
        std::cout << fragmentPath << std::endl;
        std::cout << geometryPath << std::endl;
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
    if (geometryPath == NULL)
    {
        if (linkProgram(vertexShader, fragmentShader, &program))
        {
            return LINK_PROGRAM_FAILED;
        }
    }
    else
    {
        if (compileShader(geometryCode.c_str(), GL_GEOMETRY_SHADER, &geometryShader))
        {
            return COMPILE_SHADER_FAILED;
        }
        if (linkProgram(vertexShader, fragmentShader, &program, geometryShader, true))
        {
            return LINK_PROGRAM_FAILED;
        }
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
        std::cout << "compile shader failed:" << std::endl;
        std::cout << log << std::endl;
        std::cout << source << std::endl;
        return COMPILE_SHADER_FAILED;
    }
    *result = shader;
    return 0;
}

int Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader, unsigned int *result, unsigned int geometryShader, bool hasGeometryShader)
{
    unsigned int program = glCreateProgram();
    int success;
    char log[512];
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    if (hasGeometryShader)
        glAttachShader(program, geometryShader);
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

void Shader::setUniformMatrix4(const char *name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniformMatrix3(const char *name, glm::mat3 value)
{
    glUniformMatrix3fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform1f(const char *name, float value)
{
    glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::setUniform4fv(const char *name, glm::vec4 value)
{
    glUniform4fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
}

void Shader::setUniform1i(const char *name, int value)
{
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::setUniform3fv(const char *name, glm::vec3 value)
{
    glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(value));
}

void Shader::setUniform3f(const char *name, float v1, float v2, float v3)
{
    glUniform3f(glGetUniformLocation(program, name), v1, v2, v3);
}

void Shader::setUniform4f(const char *name, float v1, float v2, float v3, float v4)
{
    glUniform4f(glGetUniformLocation(program, name), v1, v2, v3, v4);
}

Shader::~Shader()
{
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);
    glDeleteProgram(program);
}
Shader::Shader()
{
}

void Shader::setUniformBlockBinding(const char *name, unsigned int bind_point)
{
    glUniformBlockBinding(program, glGetUniformBlockIndex(program, name), bind_point);
}
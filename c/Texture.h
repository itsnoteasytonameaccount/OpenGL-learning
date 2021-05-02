#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GLWindow.h"
#include "Shader.h"

class Texture : public GLWindow
{
private:
    /* data */
    unsigned char *data;
    int width, height, nChannels;

public:
    unsigned int texture1, texture2, VAO, VBO, EBO;
    Shader *shader;
    const static int LOAD_IMAGE_FAILED = 1;
    int status;
    int loadTexture(const char *path, int id, unsigned int type, unsigned int *texture);
    int loadTexture(const char *path, int id);
    void draw();
    int createVAO();
    int createVAO(int flag);
    Texture(/* args */);
    ~Texture();
};

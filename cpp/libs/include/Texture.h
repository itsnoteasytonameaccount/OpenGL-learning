#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <iostream>

#ifndef __TEXTURE_H__
#define __TEXTURE_H__
class Texture
{
private:
    /* data */
    Texture(){

    };

public:
    const static int LOAD_IMAGE_FAILED = 1;
    static int LoadTexture(const char *path, unsigned int *texture, int id = 1, int num = 0, unsigned int wrap_t = GL_REPEAT, unsigned int wrap_s = GL_REPEAT, unsigned int min_filter = GL_LINEAR_MIPMAP_LINEAR, unsigned int mag_filter = GL_LINEAR);
    static unsigned int CreateTexture(int width, int height, unsigned int id = 1, GLint internalformat = GL_RGB, GLenum format = GL_RGB, GLenum type = GL_UNSIGNED_INT);
    static int LoadCubemaps(const char *const path[], unsigned int *texture, int id = 1, int num = 0, unsigned int wrap_t = GL_CLAMP_TO_EDGE, unsigned int warp_s = GL_CLAMP_TO_EDGE, unsigned int wrap_r = GL_CLAMP_TO_EDGE, unsigned int min_filter = GL_LINEAR, unsigned int mag_filter = GL_LINEAR);
};
#endif
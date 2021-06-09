#include <Texture.h>

int Texture::LoadCubemaps(const char *const path[], unsigned int *texture, int id, int num, unsigned int wrap_t, unsigned int wrap_s, unsigned int wrap_r, unsigned int min_filter, unsigned int mag_filter)
{
    glActiveTexture(GL_TEXTURE0 + num);
    glGenTextures(id, texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);

    int width, height, nrChannels;
    unsigned char *data;

    for (int i = 0; i < 6; i++)
    {
        data = stbi_load(path[i], &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
            {
                format = GL_RED;
            }
            else if (nrChannels == 3)
            {
                format = GL_RGB;
            }
            else if (nrChannels == 4)
            {
                format = GL_RGBA;
            }
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "load image " << path[i] << " failed" << std::endl;
            return LOAD_IMAGE_FAILED;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap_r);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, mag_filter);
    return 0;
}

int Texture::LoadTexture(const char *path, unsigned int *texture, int id, int num, unsigned int wrap_t, unsigned int wrap_s, unsigned int min_filter, unsigned int mag_filter)
{
    int width, height, nChannels;
    unsigned char *data;

    glActiveTexture(GL_TEXTURE0 + num);
    glGenTextures(id, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

    data = stbi_load(path, &width, &height, &nChannels, 0);
    if (data)
    {
        GLenum format;
        if (nChannels == 1)
        {
            format = GL_RED;
        }
        else if (nChannels == 3)
        {
            format = GL_RGB;
        }
        else if (nChannels == 4)
        {
            format = GL_RGBA;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "load image " << path << " failed" << std::endl;
        return LOAD_IMAGE_FAILED;
    }
    stbi_image_free(data);
    return 0;
}

unsigned int Texture::CreateTexture(int width, int height, unsigned int id, GLint internalformat, GLenum format, GLenum type)
{
    unsigned int texture;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(id, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return texture;
}
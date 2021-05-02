#include <GLWindow.h>
#include <Box.h>
#include <Camera.hpp>
#include <Shader.h>
#include <Texture.h>
#include <Model.hpp>
#include <Controlller.hpp>
#include <time.h>
#include "AmountCon.h"

class Instancing : public GLWindow
{
private:
    unsigned int texture1, texture2, vao, vbo, vbo2, vbo3;
    // Box box;
    Shader shader, planet_shader;
    glm::vec2 offsets[100];
    glm::mat4 *models;
    Controller con;
    Model *planet, *rock;
    int current_amount;
    /* data */
public:
    void draw();
    void getKeyInput();
    void createVAO();
    void createUniformBuffer();
    void generateModelMatrixList();
    glm::mat4 generateModelMatrix(float radius,float offset, float angle);
    void bindMeshVAO();
    Instancing(/* args */);
    ~Instancing();
};

#ifndef _MESH_H_
#define _MESH_H_
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <Shader.h>
#include <iostream>
#include <vector>
#include <assimp/scene.h>

using std::vector;

namespace mesh
{
    typedef struct
    {
        unsigned int id;
        aiString type;
        aiString path;
        /* data */
    } Texture;

    typedef struct
    {
        aiColor3D diffuse;
        aiColor3D ambient;
        aiColor3D specular;
        /* data */
    } Material;

}; // namespace mesh

class Mesh
{
private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
    long long indicesNum;
    aiMesh *mesh;
    vector<mesh::Texture> textures;
    mesh::Material material;
    bool has_texture;
    /* data */
public:
    Mesh(aiMesh *mesh, vector<mesh::Texture> &textures, mesh::Material &material, bool has_texture);
    void draw(Shader &shader);
    // Mesh(/* args */);
    ~Mesh();
};

#endif
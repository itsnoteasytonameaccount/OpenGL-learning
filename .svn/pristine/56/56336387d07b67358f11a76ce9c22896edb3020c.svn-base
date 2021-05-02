#include <glad/glad.h>
#include <glm/glm.hpp>
#include <Shader.h>
#include <iostream>
#include <vector>
#include <assimp/types.h>
#ifndef _MESH_H_
#define _MESH_H_

using std::vector;

namespace mesh
{
    typedef struct
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoord;
        /* data */
    } Vertex;

    typedef struct
    {
        unsigned int id;
        std::string type;
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
    /* data */
public:
    vector<mesh::Vertex> vertices;
    vector<mesh::Texture> textures;
    vector<unsigned int> indices;
    mesh::Material material;
    bool has_texture;
    Mesh(vector<mesh::Vertex> vertices, vector<unsigned int> indices, vector<mesh::Texture> textures, mesh::Material material, bool has_texture);
    void draw(Shader &shader);
    // Mesh(/* args */);
    ~Mesh();
};

#endif
#include "Mesh.h"

Mesh::Mesh(vector<mesh::Vertex> vertices, vector<unsigned int> indices, vector<mesh::Texture> textures, mesh::Material material, bool has_texture)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->material = material;
    this->has_texture = has_texture;

    setupMesh();
}

Mesh::~Mesh()
{
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(mesh::Vertex), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::Vertex), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::Vertex), (void *)offsetof(mesh::Vertex, Normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mesh::Vertex), (void *)offsetof(mesh::Vertex, TexCoord));

    glBindVertexArray(0);
}

void Mesh::draw(Shader &shader)
{
    unsigned int specular_index = 1, diffuse_index = 1;
    std::string name, number;
    if (has_texture)
    {
        shader.setUniform1i("material.has_texture", 1);
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            name = textures[i].type;
            if (name == "texture_diffuse")
            {
                number = std::to_string(diffuse_index++);
            }
            else if (name == "texture_specular")
            {
                number = std::to_string(specular_index++);
            }
            shader.setUniform1i(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
    }
    else
    {
        shader.setUniform1i("material.has_texture", 0);
        shader.setUniform3f("material.specular", material.specular.r, material.specular.g, material.specular.b);
        shader.setUniform3f("material.ambient", material.ambient.r, material.ambient.g, material.ambient.b);
        shader.setUniform3f("material.diffuse", material.diffuse.r, material.diffuse.g, material.diffuse.b);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

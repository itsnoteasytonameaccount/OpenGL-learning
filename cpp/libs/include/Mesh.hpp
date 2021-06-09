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
    unsigned int VBO, EBO;
    long long indicesNum;
    aiMesh *mesh;
    vector<mesh::Texture> textures;
    mesh::Material material;
    bool has_texture;

    void setupMesh()
    {
        bool hasNormals = mesh->HasNormals();
        long long verticesSize = mesh->mNumVertices * sizeof(C_STRUCT aiVector3D);
        long long normalsSize = hasNormals ? verticesSize : 0;
        long long texturesSize = mesh->mTextureCoords[0] == nullptr ? mesh->mNumVertices * 2 * sizeof(float) : verticesSize;
        long long facesSize = 0;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ARRAY_BUFFER, verticesSize * 3 + normalsSize + texturesSize, nullptr, GL_STATIC_DRAW);

        glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, mesh->mVertices);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(C_STRUCT aiVector3D), (void *)0);

        if (hasNormals)
        {
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize, normalsSize, mesh->mNormals);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(C_STRUCT aiVector3D), (void *)verticesSize);
        }

        glEnableVertexAttribArray(2);
        if (mesh->mTextureCoords[0] == NULL)
        {
            char *ptr = (char *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
            memset(ptr + verticesSize + normalsSize, 0, texturesSize);
            glUnmapBuffer(GL_ARRAY_BUFFER);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *)(verticesSize + normalsSize));
        }
        else
        {
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize, texturesSize, mesh->mTextureCoords[0]);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(C_STRUCT aiVector3D), (void *)(verticesSize + normalsSize));
        }

        if (mesh->mTangents != NULL)
        {
            glEnableVertexAttribArray(3);
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize + texturesSize, verticesSize, mesh->mTangents);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(C_STRUCT aiVector3D), (void *)(verticesSize + normalsSize + texturesSize));
        }

        if (mesh->mBitangents != NULL)
        {
            glEnableVertexAttribArray(4);
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize * 2 + normalsSize + texturesSize, verticesSize, mesh->mBitangents);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(C_STRUCT aiVector3D), (void *)(verticesSize * 2 + normalsSize + texturesSize));
        }
        indicesNum = 0;
        for (int i = 0; i < mesh->mNumFaces; i++)
        {
            indicesNum += mesh->mFaces[i].mNumIndices;
        }
        facesSize = indicesNum * sizeof(unsigned int);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, facesSize, nullptr, GL_STATIC_DRAW);

        for (int j = mesh->mNumFaces - 1; j > -1; j--)
        {
            long long indicesSize = mesh->mFaces[j].mNumIndices * sizeof(unsigned int);
            facesSize -= indicesSize;
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, facesSize, indicesSize, mesh->mFaces[j].mIndices);
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    };
    /* data */
public:
    unsigned int VAO;

    Mesh(aiMesh *mesh, vector<mesh::Texture> &textures, mesh::Material &material, bool has_texture)
    {
        this->mesh = mesh;
        this->textures = textures;
        this->material = material;
        this->has_texture = has_texture;

        setupMesh();
    };
    void draw(Shader &shader, unsigned int amount = 1)
    {
        unsigned int specular_index = 1, diffuse_index = 1, reflection_index = 1;
        std::string name, number = "";
        if (has_texture)
        {
            shader.setUniform1i("material.has_texture", 1);
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                number = "";
                glActiveTexture(GL_TEXTURE0 + i);
                name = textures[i].type.C_Str();
                if (name == "texture_diffuse")
                {
                    number = std::to_string(diffuse_index++);
                }
                else if (name == "texture_specular")
                {
                    number = std::to_string(specular_index++);
                }
                else if (name == "texture_reflection")
                {
                    number = std::to_string(reflection_index++);
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
        if (amount > 1)
        {
            glDrawElementsInstanced(GL_TRIANGLES, indicesNum, GL_UNSIGNED_INT, 0, amount);
        }
        else
        {
            glDrawElements(GL_TRIANGLES, indicesNum, GL_UNSIGNED_INT, 0);
        }
        glBindVertexArray(0);
    };
    // Mesh(/* args */);
    ~Mesh(){};
};

#endif
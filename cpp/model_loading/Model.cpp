#include "Model.h"

Model::~Model()
{
}

void Model::loadModel(std::string path)
{
    std::cout << "start importer" << std::endl;
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/') + 1);
    std::cout << "start node processor" << std::endl;
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<mesh::Vertex> vertices;
    vector<unsigned int> indices;
    vector<mesh::Texture> textures;
    mesh::Material mesh_material;
    bool has_texture = false;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        mesh::Vertex vertex;
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;
        if (mesh->HasNormals())
        {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }
        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.TexCoord = glm::vec2(0.0f);
        }
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        aiString material_name = material->GetName();
        unsigned int len = material->GetTextureCount(aiTextureType_DIFFUSE);
        if (len > 0)
        {
            has_texture = true;
            loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures, len);
            loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", textures, len);
        }
        else
        {
            aiColor3D diffuse, specular, ambient;
            material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
            mesh_material.diffuse = diffuse;
            material->Get(AI_MATKEY_COLOR_SPECULAR, specular);
            mesh_material.specular = specular;
            material->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
            mesh_material.ambient = ambient;
        }
    }

    // 匿名对象
    // 编译时只要启用了优化，返回对象似乎不会对性能有影响
    return Mesh(vertices, indices, textures, mesh_material, has_texture);
}

void Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, vector<mesh::Texture> &textures, unsigned int len)
{
    unsigned int texture_id;
    bool skip = false;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mesh::Texture texture;
        mat->GetTexture(type, i, &str);
        Texture::LoadTexture((directory + str.C_Str()).c_str(), &texture_id);
        skip = false;
        {
            for (int j = 0; j < textures_loaded.size(); j++)
                if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
        }
        if (!skip)
        {
            texture.id = texture_id;
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
        }
    }
}

void Model::draw(Shader &shader)
{
    shader.useProgram();
    for (int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader);
    }
}
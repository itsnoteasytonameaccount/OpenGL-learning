#include "Model.h"

Model::~Model()
{
}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/') + 1);
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
    vector<mesh::Texture> textures;
    mesh::Material mesh_material;
    bool has_texture = false;

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
            loadMaterialTextures(material, aiTextureType_REFLECTION, "texture_reflection", textures, len);
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

    // ????????????
    // ???????????????????????????????????????????????????????????????????????????
    return Mesh(mesh, textures, mesh_material, has_texture);
}

void Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const char *typeName, vector<mesh::Texture> &textures, unsigned int len)
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
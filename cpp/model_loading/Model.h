#include <Camera.hpp>
#include <GLWindow.h>
#include <Shader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "Mesh.h"
#include <Texture.h>

using std::string;

class Model
{
private:
    vector<Mesh> meshes;
    std::string directory;
    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    void loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, vector<mesh::Texture> &textures, unsigned int len);
    vector<mesh::Texture> textures_loaded;
    /* data */
public:
    Model(char *path)
    {
        loadModel(path);
    };
    ~Model();
    void draw(Shader &shader);
};

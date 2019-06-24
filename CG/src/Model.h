#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

#include "Shader.h"
#include "Mesh.h"
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <soil.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLint TextureFromFile(const char* path, string directory);

class Model {
public:
    /*  Functions   */
	
    Model(const char* path)
    {
        loadModel(path);
    }
	Model(void) {}

    void Draw(Shader shader);
private:
    /*  Model Data  */
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    /*  Functions   */
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                         string typeName);
};


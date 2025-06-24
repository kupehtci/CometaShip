#include "Model.h"
#include <iostream>
#include <filesystem>
#include "debug/Assertion.h"

Model::Model(const std::string& path) {
    _path = "";
    _directory = "";
    LoadModel(path);
}

void Model::LoadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | 
        aiProcess_GenNormals | 
        aiProcess_CalcTangentSpace |
        aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    _path = path;
    _directory = std::filesystem::path(path).parent_path().string();
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {

    // Iterate over model's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(ProcessMesh(mesh, scene));
    }

    // iterate over the meshes childs
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // positions
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);

        // normals
        if (mesh->HasNormals()) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }

        // set default color
        vertices.push_back(1.0f);
        vertices.push_back(1.0f);
        vertices.push_back(1.0f);

        // texture coordinates
        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        } else {
            vertices.push_back(0.0f);
            vertices.push_back(0.0f);
        }
    }

    // Process the indices of the model
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Create mesh object
    std::shared_ptr<Mesh> resultMesh = std::make_shared<Mesh>();

    resultMesh->AddVertices(vertices.data(), vertices.size());
    resultMesh->AddIndices(indices.data(), indices.size());
    resultMesh->SetLayoutBuffer({
        {0, DataType::Float3, "aPos"},
        {1, DataType::Float3, "aNormal"},
        {2, DataType::Float3, "aColor"},
        {3, DataType::Float2, "aTexCoord"}
    });
    resultMesh->Build();

    // Process material
    if (mesh != nullptr && mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::shared_ptr<Material> meshMaterial = std::make_shared<Material>();
        
        // aiString texturePath;
        // if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
        //     std::string fullPath = _directory + "/" + texturePath.C_Str();
        //     meshMaterial->SetTexture(std::make_shared<Texture>(fullPath));
        // } else {
        //     meshMaterial->SetTexture(nullptr); 
        // }
        
        // Load material textures
        LoadMaterialTextures(material, aiTextureType_DIFFUSE, meshMaterial);
        LoadMaterialTextures(material, aiTextureType_SPECULAR, meshMaterial);
        LoadMaterialTextures(material, aiTextureType_EMISSIVE, meshMaterial);
        
        // Set the shader for the material
        meshMaterial->LoadShader("Main Shader",
            "src/render/shaders/blinn_phong_shader.vert",
            "src/render/shaders/blinn_phong_shader.frag");

        // Store the material to the model's mesh
        _materials[resultMesh] = meshMaterial;
    }
    else{
        COMETA_ASSERT("[Model] Material cannot be found"); 
    }

    return resultMesh;
}

void Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::shared_ptr<Material>& material) {
    
    aiColor3D color(1.f, 1.f, 1.f);
    aiColor3D ambient(1.f, 0.5f, 0.31f);
    aiColor3D diffuse(1.f, 0.5f, 0.31f);
    aiColor3D specular(0.5f, 0.5f, 0.5f);
    float shininess = 64.0f;

    
    material->SetColor(glm::vec3(color.r, color.g, color.b));

    if(mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient) == AI_SUCCESS) {
        material->SetAmbient(glm::vec3(ambient.r, ambient.g, ambient.b));
    }
    if(mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS) {
        material->SetDiffuse(glm::vec3(diffuse.r, diffuse.g, diffuse.b));
    }
    if(mat->Get(AI_MATKEY_COLOR_SPECULAR, specular) == AI_SUCCESS) {
        material->SetSpecular(glm::vec3(specular.r, specular.g, specular.b));
    }
    if(mat->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
        material->SetShininess(shininess);
    }

    // Load textures of the specified type
    bool textureLoaded = false;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        textureLoaded = (mat->GetTexture(type, i, &str) == AI_SUCCESS);
        
        std::string texturePath = _directory + "/" + str.C_Str();
        
        switch (type) {
            case aiTextureType_DIFFUSE:
                material->LoadDiffuseMap(texturePath);
                break;
            case aiTextureType_SPECULAR:
                material->LoadSpecularMap(texturePath);
                break;
            case aiTextureType_EMISSIVE:
                material->LoadEmissionMap(texturePath);
                break;
            default:
                break;
        }
    }
    
    // Load default textures if no texture was loaded
    if (!textureLoaded) {
        switch (type) {
            case aiTextureType_DIFFUSE:
                material->LoadDiffuseMap("resources/white.jpg");  // white as default diffuse
                break;
            case aiTextureType_SPECULAR:
                material->LoadSpecularMap("resources/grey.jpg");  // grey as default specular
                break;
            case aiTextureType_EMISSIVE:
                material->LoadEmissionMap("resources/black.jpg"); // black as default emission
                break;
            default:
                break;
        }
    }
}

void Model::Draw() {
    for (auto& mesh : _meshes) {
        mesh->Draw();
    }
}
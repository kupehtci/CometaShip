#include "Material.h"


// Load maps methods
void Material::LoadDiffuseMap(const std::string& diffuseMapPath)
{
    _diffuseMap = std::make_shared<Texture>(diffuseMapPath);
    _hasDiffuseMap = true;
}
void Material::LoadSpecularMap(const std::string& specularMapPath)
{
    _specularMap = std::make_shared<Texture>(specularMapPath);
    _hasSpecularMap = true;
}

void Material::LoadEmissionMap(const std::string& emissionMapPath)
{
    _emissionMap = std::make_shared<Texture>(emissionMapPath);
    _hasEmissionMap = true;
}

void Material::LoadShader(const Shader& shader)
{
    _shader = std::make_shared<Shader>(shader);
}

void Material::LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    _shader = Shader::LoadShader(name, vertexShaderPath, fragmentShaderPath);
}

void Material::UnsetShader()
{
    _shader = nullptr;
}

// Rendering methods

/**
 * Binds imply to setting all the GPU uniforms and structures with the data stored in the object
 */
void Material::Bind(){

    _shader->Bind();
    int initialTextureIndex = 0;

    _shader->SetBool("material.hasDiffuseMap", _hasDiffuseMap);
    _shader->SetBool("material.hasSpecularMap", _hasSpecularMap);
    _shader->SetBool("material.hasEmissionMap", _hasEmissionMap);

    // Diffuse map
    if (_hasDiffuseMap){
        _shader->SetInt("material.diffuseMap", initialTextureIndex);
        _diffuseMap->Bind(initialTextureIndex);
    }

    // Specular map
    if (_hasSpecularMap)
    {
        int specularMapIndex = initialTextureIndex + static_cast<int>(_hasDiffuseMap);

        _shader->SetInt("material.specularMap", specularMapIndex);
        _specularMap->Bind(specularMapIndex);
    }

    // Emission map
    if (_hasEmissionMap){
        int emissionMapIndex = initialTextureIndex + static_cast<int>(_hasDiffuseMap) + static_cast<int>(_hasSpecularMap);

        _shader->SetInt("material.emissionMap", emissionMapIndex);
        _emissionMap->Bind(emissionMapIndex);
    }

    // Properties set up in shader
    _shader->SetFloat3("material.color", _color);
    _shader->SetFloat3("material.ambient", _ambient);
    _shader->SetFloat3("material.diffuse", _diffuse);
    _shader->SetFloat3("material.specular", _specular);
    _shader->SetFloat("material.shininess", _shininess);
}

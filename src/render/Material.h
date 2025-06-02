#ifndef COMETA_MATERIAL_H
#define COMETA_MATERIAL_H

#include <glm.hpp>
#include "render/Texture.h"
#include "render/Shader.h"

/**
 * Material class that doesn't have light maps, only values
 */
class SimpleMaterial{
private:
    glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 _ambient = glm::vec3(1.0f, 0.5f, 0.32f);
    glm::vec3 _diffuse = glm::vec3(1.0f, 0.5f, 0.32f);
    glm::vec3 _specular = glm::vec3(0.5f, 0.5f, 0.5f);
    float _shininess = 256.0f;


public:
	SimpleMaterial() = default;
    SimpleMaterial(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
        _color(color), _ambient(ambient), _diffuse(diffuse), _specular(specular), _shininess(shininess) {}


    // --------- GETTERS AND SETTERS ---------
    glm::vec3 GetColor() { return _color; }
    glm::vec3 GetAmbient() { return _ambient; }
    glm::vec3 GetDiffuse() { return _diffuse; }
    glm::vec3 GetSpecular() { return _specular; }
    float GetShininess() { return _shininess; }
};

/**
 * Material class with Diffuse map and Specular map
 */
class Material
{
private:
    // Material properties
    glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 _ambient = glm::vec3(1.0f, 0.5f, 0.32f);
    glm::vec3 _diffuse = glm::vec3(1.0f, 0.5f, 0.32f);
    glm::vec3 _specular = glm::vec3(0.5f, 0.5f, 0.5f);
    float _shininess = 256.0f;

    // Maps
    std::shared_ptr<Texture> _diffuseMap = nullptr;
    std::shared_ptr<Texture> _specularMap = nullptr;
    std::shared_ptr<Texture> _emissionMap = nullptr;

    // checks
    bool _hasDiffuseMap = false;
    bool _hasSpecularMap = false;
    bool _hasEmissionMap = false;

    // Shader properties
    std::shared_ptr<Shader> _shader = nullptr;

public:

    // Constructors
    Material() = default;
    Material(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) :
        _color(color), _ambient(ambient), _diffuse(diffuse), _specular(specular), _shininess(shininess)
    {
        _hasDiffuseMap = false;
        _hasSpecularMap = false;
        _hasEmissionMap = false;
    }

    Material(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess,
        const std::string& diffuseMapPath, const std::string& specularMapPath, const std::string& emissionMapPath) :
        _color(color), _ambient(ambient), _diffuse(diffuse), _specular(specular), _shininess(shininess)
    {
        LoadDiffuseMap(diffuseMapPath);
        LoadSpecularMap(specularMapPath);
        LoadEmissionMap(emissionMapPath);
        _hasDiffuseMap = true;
        _hasSpecularMap = true;
        _hasEmissionMap = true;
    }

    // Load maps methods
    void LoadDiffuseMap(const std::string& diffuseMapPath);
    void LoadSpecularMap(const std::string& specularMapPath);
    void LoadEmissionMap(const std::string& emissionMapPath);

    // Shader methods
    void LoadShader(const Shader& shader);
    void LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void UnsetShader();

    // Rendering methods
    void Bind();


    // --------- SETTERS ---------
    void SetColor(glm::vec3 color) { _color = color; }
    void SetAmbient(glm::vec3 ambient) { _ambient = ambient; }
    void SetDiffuse(glm::vec3 diffuse) { _diffuse = diffuse; }
    void SetSpecular(glm::vec3 specular) { _specular = specular; }
    void SetShininess(float shininess) {_shininess = shininess; }

    void SetHasDiffuseMap(bool value) {_hasDiffuseMap = value; }
    void SetHasSpecularMap(bool value) {_hasSpecularMap = value; }
    void SetHasEmissionMap(bool value) {_hasEmissionMap = value; }

    // --------- GETTERS ---------
    // Material properties getters
    [[nodiscard]] glm::vec3 GetColor() const { return _color; }
    [[nodiscard]] glm::vec3 GetAmbient() const { return _ambient; }
    [[nodiscard]] glm::vec3 GetDiffuse() const { return _diffuse; }
    [[nodiscard]] glm::vec3 GetSpecular() const { return _specular; }
    [[nodiscard]] float GetShininess() const { return _shininess; }

    // Maps getters
    [[nodiscard]] inline std::shared_ptr<Texture> GetDiffuseMap() const { return _diffuseMap; }
    [[nodiscard]] inline std::shared_ptr<Texture> GetSpecularMap() const { return _specularMap; }
    [[nodiscard]] inline std::shared_ptr<Texture> GetEmissionMap() const { return _emissionMap; }

    [[nodiscard]] inline bool HasDiffuseMap() const { return _hasDiffuseMap; }
    [[nodiscard]] inline bool HasSpecularMap() const { return _hasSpecularMap; }
    [[nodiscard]] inline bool HasEmissionMap() const { return _hasEmissionMap; }

    // Shader getters
    [[nodiscard]] std::shared_ptr<Shader> GetShader() const { return _shader; }
};

#endif
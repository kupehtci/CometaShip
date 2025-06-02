//
// Created by Daniel Laplana Gimeno on 30/1/25.
//

#include "Shader.h"
#include <filesystem>
#include "debug/Assertion.h"

// Implement the cache for the shaders
// Static member
std::unordered_map<std::string, std::shared_ptr<Shader>> Shader::_shadersCache = std::unordered_map<std::string, std::shared_ptr<Shader>>(20);


Shader::Shader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource){
    _debugName = name;
    _shaderUID = 0;

    _shaderSources[GL_VERTEX_SHADER] = LoadFromFile(vertexShaderSource);
    _shaderSources[GL_FRAGMENT_SHADER] = LoadFromFile(fragmentShaderSource);

    _filePaths[GL_VERTEX_SHADER] = vertexShaderSource;
    _filePaths[GL_FRAGMENT_SHADER] = fragmentShaderSource;

    // std::cout << "Shader constructor called for " << name << std::endl;
    CompileShaderProgram();
}

Shader::Shader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& geometryShaderSource){
    _debugName = name;
    _shaderUID = 0;

    _shaderSources[GL_VERTEX_SHADER] = LoadFromFile(vertexShaderSource);
    _shaderSources[GL_FRAGMENT_SHADER] = LoadFromFile(fragmentShaderSource);
    _shaderSources[GL_GEOMETRY_SHADER] = LoadFromFile(geometryShaderSource);

    _filePaths[GL_VERTEX_SHADER] = vertexShaderSource;
    _filePaths[GL_FRAGMENT_SHADER] = fragmentShaderSource;
    _filePaths[GL_GEOMETRY_SHADER] = geometryShaderSource;

    CompileShaderProgram();
}

Shader::~Shader(){
    _shaderSources.clear();
    _filePaths.clear();
    Delete();
}

// ------------ CACHED METHODS ------------

// Static method to load shaders
std::shared_ptr<Shader> Shader::LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    const std::string key = vertexShaderPath + "__" + fragmentShaderPath;

    // std::unordered_map<std::string,std::shared_ptr<Shader>>::iterator
    auto cacheIterator = _shadersCache.find(key);

    if (cacheIterator != _shadersCache.end()){
        return cacheIterator->second;
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(name, vertexShaderPath, fragmentShaderPath);
    _shadersCache.insert( {key,shader});
    return shader;
}

// Static method to load shaders with geometry shader
std::shared_ptr<Shader> Shader::LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath)
{
    const std::string key = vertexShaderPath + "__" + fragmentShaderPath + "__" + geometryShaderPath;

    auto cacheIterator = _shadersCache.find(key);

    if (cacheIterator != _shadersCache.end()){
        return cacheIterator->second;
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(name, vertexShaderPath, fragmentShaderPath, geometryShaderPath);
    _shadersCache.insert( {key,shader});
    return shader;
}

void Shader::Debug()
{
    unsigned int i = 0;
    for (auto shader : _shadersCache)
    {
        std::cout << "Shader cached [" << i << "] : " << shader.second->_debugName << std::endl;
        std::cout << "   VERTEX SHADER     source code path: " << shader.second->GetFilePath(GL_VERTEX_SHADER) << std::endl;
        std::cout << "   FRAGMENT SHADER   source code path: " << shader.second->GetFilePath(GL_FRAGMENT_SHADER) << std::endl;
    }
}


// ------------ UNIFORM METHODS ------------

void Shader::SetBool(const std::string& variableName, const bool& value) const{
    int location = glGetUniformLocation(_shaderUID, variableName.c_str());
    glUniform1i(location,static_cast<int>(value));
}

void Shader::SetFloat(const std::string& variableName, const float& value) const{
    int location = glGetUniformLocation(_shaderUID, variableName.c_str());
    glUniform1f(location, value);
}

void Shader::SetFloat2(const std::string &variableName, const glm::vec2 &value) const {
    int location = glGetUniformLocation(_shaderUID, variableName.c_str());
    glUniform2f(location, value.x, value.y);
}

void Shader::SetFloat3(const std::string& variableName, const glm::vec3& value) const{
    int location = glGetUniformLocation(_shaderUID, variableName.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetFloat4(const std::string& variableName, const glm::vec4& value) const{
    int location = glGetUniformLocation(_shaderUID, variableName.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetInt(const std::string& variableName, const int& value) const{
    int location = glGetUniformLocation(_shaderUID, variableName.c_str());
    glUniform1i(location, value);
}

void Shader::SetIntArray(const std::string& variableName, const int* values, uint32_t count) const{
    int location = glGetUniformLocation(_shaderUID, variableName.c_str());
    glUniform1iv(location, count, values);
}

void Shader::SetMatrix4(const std::string& variableName, const glm::mat4& value) const{
    int location = glGetUniformLocation(_shaderUID, variableName.c_str()); 
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}



std::string Shader::LoadFromFile(std::string filePath){

    std::ifstream file = std::ifstream(filePath);

    if(!file.is_open()){
        std::cout << "ERROR/SHADER/SOURCE/COULDNT_OPEN_FILE: " << filePath << std::endl;
        return "";
    }

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    // Set as not compiled to handle new compilation
    _isCompiled = false;

    return stream.str();
}

unsigned int Shader::CompileShader(GLenum shaderType) {

    unsigned int uid = glCreateShader(shaderType);
    const char* shaderSource = _shaderSources[shaderType].c_str();
    glShaderSource(uid, 1, &shaderSource, NULL);
    glCompileShader(uid);

    // Validate shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(uid, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(uid, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    // _shaderUID = uid;
    _isCompiled = true;
    return uid;
}

void Shader::CompileShaderProgram(){
    std::vector<GLuint> shaderIDs;

    unsigned int shaderProgramID = glCreateProgram();

    for(const auto& [shaderType, shaderSource] : _shaderSources){
        // std::cout << "Reading shader in shader sources " << shaderType << " with source: " << std::endl << shaderSource << std::endl;
        unsigned int shaderUID = CompileShader(shaderType);
        shaderIDs.emplace_back(shaderUID);

        glAttachShader(shaderProgramID, shaderUID);
    }

    glLinkProgram(shaderProgramID);

    // Validate Shader Program compilation
    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        std::cout << "ERROR/SHADER/PROGRAM/LINKING_FAILED" << std::endl << infoLog << std::endl;
        return;
    }

    // Detach and delete the shaders compiled
    for(GLuint shaderUID : shaderIDs){
        glDetachShader(shaderProgramID, shaderUID);
        glDeleteShader(shaderUID);
    }

    _shaderUID = shaderProgramID;
}

void Shader::Bind(){
    if (!_isCompiled)
    {
        COMETA_ERROR(" [SHADER] Tried to bind a non compiled shader");
        return;
    }
    glUseProgram(_shaderUID);
}

void Shader::Unbind() {
    glUseProgram(0);
}

void Shader::Delete(){
    if(_shaderUID == 0){
        std::cout << "ERROR::SHADER::Trying to delete a not compiled shader" << std::endl;
    }
    else
    {
        glDeleteProgram(_shaderUID);
    }

    _shaderUID = 0;
}
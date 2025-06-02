//
// Created by Daniel Laplana Gimeno on 30/1/25.
//

#ifndef AURAGL_SHADER_H
#define AURAGL_SHADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

/**
 * Class that represents a GSLS Shader and offers the utility to use them
 */
class Shader {

private:
    unsigned int _shaderUID;                                        // UID of the shader once it has been compiled
    std::unordered_map<GLenum, std::string> _shaderSources;         // Store as a map (Shader type - source code)
    std::unordered_map<GLenum, std::string> _filePaths;             // Store as a map (Shader type - file path)
    std::string _debugName;                                         // Name for debugging purposes or UI
    bool _isCompiled = false;                                       // Keep track if Render backend has the shader compiled

    static std::unordered_map<std::string, std::shared_ptr<Shader>> _shadersCache;    // Cache of all the shaders

public:

    Shader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    Shader(const std::string& name, const std::string& vertexShaderSource, const std::string& fragmentShaderSource, const std::string& geometryShaderSource);
    ~Shader();

    // ------------ CACHED METHODS ------------
    // Chached methods are the most recomendable way to create and load a shader program.
    // Invoked as an static method --> Shader shader = Shader::LoadShader(...)

    static std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    static std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath);
    static void Debug();

    // ------------ UNIFORMS METHODS ------------
        
    /**
     * Set the value of a boolean uniform variable
     * this boolean is passed as an int to the shader
     * @param variableName (std::string) name of the uniform variable to set
     * @param value (bool) new value of the variable
     */
    void SetBool(const std::string& variableName, const bool& value)const;

    /**
     * Set the value of a float uniform variable
     * @param variableName (std::string) name of the uniform variable to set
     * @param value (float) new value of the variable
     */
    void SetFloat(const std::string& variableName, const float& value) const;

    /**
     * Set the value of a 2 float vector uniform variable
     * @param variableName (std::string) name of the uniform variable to set
     * @param value (glm::vec2) new value of the variable
     */
    void SetFloat2(const std::string& variableName, const glm::vec2& value) const;

    /**
     * Set the value of a 3 floats vector uniform variable
     * @param variableName (std::string) name of the uniform variable to set
     * @param value (glm::vec3) new value of the variable
     */
    void SetFloat3(const std::string& variableName, const glm::vec3& value) const;

    /**
     * Set the value of a 4 floats vector uniform variable
     * @param variableName (std::string) name of the uniform variable to set
     * @param value (glm::vec4) new value of the variable
     */
    void SetFloat4(const std::string& variableName, const glm::vec4& value) const;

    /**
     * Set the value of an int  uniform variable
     * @param variableName (std::string) name of the uniform variable to set
     * @param value (int) new value of the variable
     */
    void SetInt(const std::string& variableName, const int& value) const;

    /**
     * Set the value of an array of int uniform variable
     * You need to pass a pointer to an array of values and the count of values
     * @param variableName (std::string) name of the uniform variable to set
     * @param values (const int*) new value of the variable
     * @param count (uint32_t) number of values within the int array
     */
    void SetIntArray(const std::string& variableName, const int* values, uint32_t count) const;

    /**
     * Set the value of an Matrix 4 x 4 uniform variable
     * The matrix is then transformed to a pointer of the first element
     * @param variableName (std::string) name of the uniform variable to set
     * @param values (glm::mat4&) Matrix 4 x 4 new value of the variable
     */
    void SetMatrix4(const std::string& variableName, const glm::mat4& value) const; 

private:
    /**
     * Load a shader from a file using an stream and retunrns the shader as an string
     * @param filePath Path to the file that is being loaded as a shader
     * @return (string) shader loaded as string
     */
    std::string LoadFromFile(std::string filePath);

    /**
     * Create a shader buffer in OpenGL, link the source and compile it
     * Once it has been generated it returns the UID of the buffered shader
     * If shader has not been loaded in Shader's source or some error ocurred during shader compilation it will return 0
     * @return (unsigned int) UID of the shader compilated
     */
    unsigned int CompileShader(GLenum shaderType);

    /**
     * Compile the shader program by compiling each one of the shaders and then link them into the program
     * Once are all linked the shaders are detached and deleted its compilation
     */
    void CompileShaderProgram();


public:

    /**
     * Bind the shader
     */
    void Bind();

    /**
     * Unbind the shader
     */
    void Unbind();

    /**
     * Delete the shader program in OpenGL by its reference
     */
     void Delete();

    // ----------------- GETTERS AND SETTERS -----------------

public:
    [[nodiscard]] std::string GetSourceCode(GLenum shaderType){ return _shaderSources[shaderType]; }
    [[nodiscard]] std::string GetFilePath(GLenum shaderType){ return _filePaths[shaderType];}
    [[nodiscard]] unsigned int GetShaderUID() const{ return _shaderUID; }
    [[nodiscard]] inline bool IsCompiled() const{ return _isCompiled; }

};


#endif //AURAGL_SHADER_H

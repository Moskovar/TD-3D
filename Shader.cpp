#include "Shader.h"
#include <iostream>
#include <fstream>~
#include <sstream>

Shader::Shader(std::string vertexSrc, std::string fragmentSrc)
{
    shaderProgram = createShaderProgram(vertexSrc, fragmentSrc);
}

std::string Shader::readShaderSource(std::string filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open()) 
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint Shader::compileShader(const char* source, GLenum shaderType)
{
    {
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        // Vérifier les erreurs de compilation
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        return shader;
    }
}

GLuint Shader::createShaderProgram(std::string vertexPath, std::string fragmentPath)
{
    std::string vertexCode = readShaderSource(vertexPath);
    std::string fragmentCode = readShaderSource(fragmentPath);

    GLuint vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Vérifier les erreurs de linkage
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Supprimer les shaders car ils sont maintenant liés dans le programme
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
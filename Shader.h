#pragma once
#include <string>
#include <GL/glew.h>

class Shader
{
	public:
		Shader() {};
		Shader(std::string vertexSrc, std::string fragmentSrc);

		GLuint getShaderProgram() { return shaderProgram; }

	private:
		GLuint shaderProgram = 0;

		std::string readShaderSource(std::string filePath);
		GLuint compileShader(const char* source, GLenum shaderType);
		GLuint createShaderProgram(std::string vertexPath, std::string fragmentPath);
};


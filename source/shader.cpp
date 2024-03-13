#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

#include "shader.h"


ShaderProgram::ShaderProgram()
	: handle(0) {}

ShaderProgram::~ShaderProgram() {}


bool ShaderProgram::Create(const char* vertexPath, const char* fragmentPath) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}

	catch (std::ifstream::failure&) {
		return false;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");
	// shader Program
	handle = glCreateProgram();
	glAttachShader(handle, vertex);
	glAttachShader(handle, fragment);
	glLinkProgram(handle);
	CheckCompileErrors(handle, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return true;
}


void ShaderProgram::Destroy() {
	if (handle) {
		glDeleteProgram(handle);
		handle = 0;
	}
}

void ShaderProgram::Bind() {
	if (handle)
		glUseProgram(handle);
}

void ShaderProgram::Unbind() {
	glUseProgram(0);
}


void ShaderProgram::SetUniform1i(const char* name, int v0) {
	glUniform1i(glGetUniformLocation(handle, name), v0);
}

void ShaderProgram::SetUniform2i(const char* name, int v0, int v1) {
	glUniform2i(glGetUniformLocation(handle, name), v0, v1);
}

void ShaderProgram::SetUniform3i(const char* name, int v0, int v1, int v2) {
	glUniform3i(glGetUniformLocation(handle, name), v0, v1, v2);
}

void ShaderProgram::SetUniform4i(const char* name, int v0, int v1, int v2, int v3) {
	glUniform4i(glGetUniformLocation(handle, name), v0, v1, v2, v3);
}

void ShaderProgram::SetUniform1f(const char* name, float v0) {
	glUniform1f(glGetUniformLocation(handle, name), v0);
}

void ShaderProgram::SetUniform2f(const char* name, float v0, float v1) {
	glUniform2f(glGetUniformLocation(handle, name), v0, v1);
}

void ShaderProgram::SetUniform3f(const char* name, float v0, float v1, float v2) {
	glUniform3f(glGetUniformLocation(handle, name), v0, v1, v2);
}

void ShaderProgram::SetUniform4f(const char* name, float v0, float v1, float v2, float v3) {
	glUniform4f(glGetUniformLocation(handle, name), v0, v1, v2, v3);
}

void ShaderProgram::SetUniform1iv(const char* name, int count, const int* v) {
	glUniform1iv(glGetUniformLocation(handle, name), count, v);
}

void ShaderProgram::SetUniform2iv(const char* name, int count, const int* v) {
	glUniform2iv(glGetUniformLocation(handle, name), count, v);
}

void ShaderProgram::SetUniform3iv(const char* name, int count, const int* v) {
	glUniform3iv(glGetUniformLocation(handle, name), count, v);
}

void ShaderProgram::SetUniform4iv(const char* name, int count, const int* v) {
	glUniform4iv(glGetUniformLocation(handle, name), count, v);
}

void ShaderProgram::SetUniform1fv(const char* name, int count, const float* v) {
	glUniform1fv(glGetUniformLocation(handle, name), count, v);
}

void ShaderProgram::SetUniform2fv(const char* name, int count, const float* v) {
	glUniform2fv(glGetUniformLocation(handle, name), count, v);
}

void ShaderProgram::SetUniform3fv(const char* name, int count, const float* v) {
	glUniform3fv(glGetUniformLocation(handle, name), count, v);
}

void ShaderProgram::SetUniform4fv(const char* name, int count, const float* v) {
	glUniform4fv(glGetUniformLocation(handle, name), count, v);
}

void ShaderProgram::SetUniformMatrix2fv(const char* name, int count, const float* v) {
	glUniformMatrix2fv(glGetUniformLocation(handle, name), count, true, v);
}

void ShaderProgram::SetUniformMatrix3fv(const char* name, int count, const float* v) {
	glUniformMatrix3fv(glGetUniformLocation(handle, name), count, true, v);
}

void ShaderProgram::SetUniformMatrix4fv(const char* name, int count, const float* v) {
	glUniformMatrix4fv(glGetUniformLocation(handle, name), count, true, v);
}


void ShaderProgram::CheckCompileErrors(unsigned int shader, std::string type) {
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
                         "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << 
                         "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

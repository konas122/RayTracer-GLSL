#ifndef SHADER_H
#define SHADER_H

#include <string>


class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();

	bool Create(const char* vertexPath, const char* fragmentPath);
	void Destroy();
	void Bind();
	void Unbind();

	void SetUniform1i(const char* name, int v0);
	void SetUniform2i(const char* name, int v0, int v1);
	void SetUniform3i(const char* name, int v0, int v1, int v2);
	void SetUniform4i(const char* name, int v0, int v1, int v2, int v3);
	void SetUniform1f(const char* name, float v0);
	void SetUniformMatrix2fv(const char* name, int count, const float* v);
	void SetUniformMatrix3fv(const char* name, int count, const float* v);
	void SetUniformMatrix4fv(const char* name, int count, const float* v);

    void SetUniform2f(const char *name, float v0, float v1);
    void SetUniform3f(const char *name, float v0, float v1, float v2);
    void SetUniform4f(const char *name, float v0, float v1, float v2, float v3);
    void SetUniform1iv(const char *name, int count, const int *v);
    void SetUniform2iv(const char *name, int count, const int *v);
    void SetUniform3iv(const char *name, int count, const int *v);
    void SetUniform4iv(const char *name, int count, const int *v);
    void SetUniform1fv(const char *name, int count, const float *v);
    void SetUniform2fv(const char *name, int count, const float *v);
    void SetUniform3fv(const char *name, int count, const float *v);
    void SetUniform4fv(const char *name, int count, const float *v);

private:
    void CheckCompileErrors(unsigned int shader, std::string type);

private:
    unsigned int handle;
};


#endif

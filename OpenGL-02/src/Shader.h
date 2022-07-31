#pragma once
#include <string>
#include <unordered_map>
using namespace std;

#include "glm/glm.hpp"

struct ShaderProgramSource
{
	string  VertexSource;
	string  FragmentSource;
};

class Shader
{
private:
	string m_FilePath;
	unsigned int m_RendererID;
	//caching for uniforms用于存储统一变量的缓存系统
	unordered_map<string, int> m_UniformLocationCache;
public:
	Shader(const string &filepath);
		
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms
	void SetUniform1i(const string &name, int value);
	void SetUniform1f(const string &name, float value);

	void SetUniform4f(const string &name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const string &name,const glm::mat4& matr);

private:
	ShaderProgramSource ParseShader(const string& filepath);
	unsigned int CompileShader(unsigned int type, const string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& framentShader);
	 int GetUniformLocation(const string &name);
};
#pragma once
#include <string>
#include <unordered_map>
using namespace std;

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
	//caching for uniforms���ڴ洢ͳһ�����Ļ���ϵͳ
	unordered_map<string, int> m_UniformLocationCache;
public:
	Shader(const string &filepath);
		
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms
	void SetUniform4f(const string &name, float v0, float v1, float v2, float v3);
private:
	ShaderProgramSource ParseShader(const string& filepath);
	unsigned int CompileShader(unsigned int type, const string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& framentShader);
	unsigned int GetUniformLocation(const string &name);
};
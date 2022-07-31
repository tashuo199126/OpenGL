#include<iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

#include "Shader.h"
#include "Renderer.h"



Shader::Shader(const string &filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	 
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));//��ID��Ϊ0ʱ�Ŵ����ɹ�
}


 ShaderProgramSource Shader::ParseShader(const string& filepath)
{
	ifstream stream(filepath);

	//��Ϊ��ɫ�����͵�ö����
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	string line;
	stringstream ss[2];//���������ַ��������͵Ķ�ջ�����㡢Ƭ�θ�һ��
	ShaderType type = ShaderType::NONE;
	//���ַ��������ַ����ͱ�����
	while (getline(stream, line))
	{
		if (line.find("#shader") != string::npos)
		{
			//������������ɫ��
			if (line.find("vertex") != string::npos)
				type = ShaderType::VERTEX;
			//������Ƭ����ɫ��
			else if (line.find("fragment") != string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';//������������������ 
		}
	}
	return { ss[0].str(), ss[1].str() };//���ַ������ַ���������
}

 //������ɫ��
 unsigned int Shader::CompileShader(unsigned int type, const string & source)
 {
	 unsigned int id = glCreateShader(type);
	 const char* src = source.c_str();//ָ��ʵ��Դ�ַ����ݿ�ͷָ�룬����һ��ָ��std�ַ��������ݵ�ָ��
	 //const char* src = GetString.c_str();����һ����ʱ�ַ�������ߵ�ָ��������ַ������������Խ�������¼���ָ��
	 glShaderSource(id, 1, &src, nullptr);//��ָ����ɫ�����滻��ɫ�������е�Դ����
	 glCompileShader(id);

	 int result;//��ʾ����״̬
	 //��ѯ�ú������Ƿ��д�
	 glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	 //��ɫ������ʧ�ܣ���ȡʧ����Ϣ ���´������// TODO : Error handing
	 if (result == GL_FALSE)
	 {
		 int length;
		 glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		 char *message = (char*)alloca(length * sizeof(char));//��̬��ʹ�ö�ջ�����ַ�
		 glGetShaderInfoLog(id, length, &length, message);//��ȡ��ɫ������Ϣ��־
		 cout << "Failed to compile shader " <<
			 (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << endl;
		 cout << message << endl;
		 glDeleteShader(id);
		 return 0;//������û�ɹ�����0����Ϊ���������棬���Բ��ܷ���-1
	 }


	 return id;
 }




 //����ɫ����Դ������ΪvertexShader���ַ������գ�
  unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& framentShader)
  {
	 //�ú��������ܶ����������ã������κζ�������һ��ָ��ָ������������һ���޷�������
	 unsigned int program = glCreateProgram();
	 //������ɫ������
	 //������ɫ��
	 unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);  //GL_VERTEX_SHADER�޷�������
	 //Ƭ����ɫ��
	 unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, framentShader);

	 //������������ɫ�������Ӹ��ӵ���Ҫ�ĳ���
	 //�������Σ�ÿ����ɫ����һ��
	 glAttachShader(program, vs);
	 glAttachShader(program, fs);

	 //���ӳ���
	 glLinkProgram(program);
	 glValidateProgram(program);

	 glDeleteShader(vs);
	 glDeleteShader(fs);

	 return program;
 }

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const string &name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const string &name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	GLCall(unsigned int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)//uniform�ĵ�ַ������
		cout << " Warning : uniform" << name << " doesn' t exist!" << endl;
	
		m_UniformLocationCache[name] = location;
	return location;
}

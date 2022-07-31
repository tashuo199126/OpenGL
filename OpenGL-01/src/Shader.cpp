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
	GLCall(glDeleteProgram(m_RendererID));//当ID不为0时才创建成功
}


 ShaderProgramSource Shader::ParseShader(const string& filepath)
{
	ifstream stream(filepath);

	//名为着色器类型的枚举类
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	string line;
	stringstream ss[2];//创建两个字符串流类型的堆栈，顶点、片段各一个
	ShaderType type = ShaderType::NONE;
	//逐字符串按照字符类型遍历，
	while (getline(stream, line))
	{
		if (line.find("#shader") != string::npos)
		{
			//遍历至顶点着色器
			if (line.find("vertex") != string::npos)
				type = ShaderType::VERTEX;
			//遍历至片段着色器
			else if (line.find("fragment") != string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';//将类型用作数组索引 
		}
	}
	return { ss[0].str(), ss[1].str() };//将字符串从字符串流返回
}

 //创建着色器
 unsigned int Shader::CompileShader(unsigned int type, const string & source)
 {
	 unsigned int id = glCreateShader(type);
	 const char* src = source.c_str();//指向实际源字符数据开头指针，返回一个指向std字符串内数据的指针
	 //const char* src = GetString.c_str();返回一种临时字符串，左边的指针可能在字符串被清理或者越界的情况下继续指向
	 glShaderSource(id, 1, &src, nullptr);//它指定着色器，替换着色器对象中的源代码
	 glCompileShader(id);

	 int result;//表示编译状态
	 //查询该函数看是否有错
	 glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	 //着色器编译失败，获取失败信息 以下代码代替// TODO : Error handing
	 if (result == GL_FALSE)
	 {
		 int length;
		 glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		 char *message = (char*)alloca(length * sizeof(char));//动态的使用堆栈分配字符
		 glGetShaderInfoLog(id, length, &length, message);//获取着色器的信息日志
		 cout << "Failed to compile shader " <<
			 (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << endl;
		 cout << message << endl;
		 glDeleteShader(id);
		 return 0;//编译器没成功返回0，因为被带到外面，所以不能返回-1
	 }


	 return id;
 }




 //将着色器的源代码作为vertexShader等字符串接收，
  unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& framentShader)
  {
	 //该函数不接受对整数的引用，或别的任何东西像是一个指针指向整数，返回一个无符号整数
	 unsigned int program = glCreateProgram();
	 //创建着色器对象
	 //顶点着色器
	 unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);  //GL_VERTEX_SHADER无符号整型
	 //片段着色器
	 unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, framentShader);

	 //将上面两个着色器，连接附加到想要的程序
	 //连接两次，每个着色器各一次
	 glAttachShader(program, vs);
	 glAttachShader(program, fs);

	 //链接程序
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
	if (location == -1)//uniform的地址不存在
		cout << " Warning : uniform" << name << " doesn' t exist!" << endl;
	
		m_UniformLocationCache[name] = location;
	return location;
}

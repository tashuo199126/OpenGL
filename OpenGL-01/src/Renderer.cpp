#include<iostream>
#include"Renderer.h"

using namespace std;

void GLClearError()
{
	//清除上一个函数的错误
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	//只要有错已知循环
	while (GLenum error = glGetError())
	{
		cout << "[OpenGL Error] (" << error << ")" << function <<
			" " << file << ";" << line << endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);

}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}

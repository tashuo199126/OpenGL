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
	//将程序（着色器）和GPU绑定，以用于渲染
	shader.Bind();
	//顶点数组包含要处理的图形数据
	va.Bind();
	//索引缓冲区包含顶点到顶点缓冲区中的索引
	ib.Bind();
	//下面语句将通过缓冲区访问顶点缓冲区，分别在顶点上调用着色器程序
	//最终实现在屏幕上光栅化的绘制 
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}

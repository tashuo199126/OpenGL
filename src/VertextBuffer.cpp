#include "VerTextBuffer.h"
#include"Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID));//生成一个缓冲区，glGenBuffers之类的指令是函数指针。
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//开辟缓存，绑定，将一个指针指向这里
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	//将上面定义的位置复制到实际缓冲区中， 6 * sizeof(float)创建一个指向他们的指针并标明它们有多大
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind()const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind()const 
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
 
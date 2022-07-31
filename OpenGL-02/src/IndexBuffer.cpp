#include "IndexBuffer.h"
#include"Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int*data, unsigned int count)
	:m_Count(count)
{
	//ASSERT(sizeof(un signed int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_RendererID));//生成一个缓冲区，glGenBuffers之类的指令是函数指针。
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));//开辟缓存，绑定，将一个指针指向这里
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));
	//将上面定义的位置复制到实际缓冲区中， 6 * sizeof(float)创建一个指向他们的指针并标明它们有多大
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

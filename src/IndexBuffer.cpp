#include "IndexBuffer.h"
#include"Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int*data, unsigned int count)
	:m_Count(count)
{
	//ASSERT(sizeof(un signed int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_RendererID));//����һ����������glGenBuffers֮���ָ���Ǻ���ָ�롣
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));//���ٻ��棬�󶨣���һ��ָ��ָ������
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW));
	//�����涨���λ�ø��Ƶ�ʵ�ʻ������У� 6 * sizeof(float)����һ��ָ�����ǵ�ָ�벢���������ж��
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

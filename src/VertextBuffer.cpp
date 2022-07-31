#include "VerTextBuffer.h"
#include"Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID));//����һ����������glGenBuffers֮���ָ���Ǻ���ָ�롣
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//���ٻ��棬�󶨣���һ��ָ��ָ������
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	//�����涨���λ�ø��Ƶ�ʵ�ʻ������У� 6 * sizeof(float)����һ��ָ�����ǵ�ָ�벢���������ж��
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
 
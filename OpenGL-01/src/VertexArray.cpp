#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));//����һ���������飬��ָ��ָ��
	//GLCall(glBindVertexArray(m_RendererID));//�󶨶������飨ID�� 
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
	Bind();//�󶨶�������
	vb.Bind();//�󶨶��㻺����
	//���ö��㲼��
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;//ƫ����
	for (unsigned int i = 0; i<elements.size();i++)
	{
		const auto& element = elements[i];
		//����OpenGL���㻺��������
		GLCall(glEnableVertexAttribArray(i));//��������Ϊ0�Ķ���/Ϊ�����λ�á��������߷ֱ��������������GPU��ȡ��
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfTypre(element.type);
		//sizeof(float) * 2�����㡱�ֽڣ���λ�á��������� ��ƫ���������㵽�����ƫ����
		//�������0��ָ�룩������ڶ�����ʼ����λ�õ�ƫ����eg����const void*��8�� ��һ�������ڣ���ʼ���Ե��������Ե�ƫ���� 
		//��һ�����Դ��������������0���󶨵�GL_ARRAY_BUFFER
		//����֮�ö������飨vao�������˶��������������飨��196���Ͷ��㻺������192��
	}
	
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}


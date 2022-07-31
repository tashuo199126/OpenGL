#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));//开辟一个顶点数组，并指向指针
	//GLCall(glBindVertexArray(m_RendererID));//绑定顶点数组（ID） 
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
	Bind();//绑定顶点数组
	vb.Bind();//绑定顶点缓冲区
	//设置顶点布局
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;//偏移量
	for (unsigned int i = 0; i<elements.size();i++)
	{
		const auto& element = elements[i];
		//告诉OpenGL顶点缓存区布局
		GLCall(glEnableVertexAttribArray(i));//调用索引为0的顶点/为顶点的位置、纹理、法线分别给予索引。便于GPU读取。
		GLCall(glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfTypre(element.type);
		//sizeof(float) * 2“顶点”字节（含位置、纹理。。。 ）偏移量，顶点到顶点的偏移量
		//最后属性0（指针）【相较于顶点起始属性位置的偏移量eg：（const void*）8】 是一个顶点内，起始属性到后面属性的偏移量 
		//第一个属性代表顶点数组的索引0，绑定到GL_ARRAY_BUFFER
		//换言之用顶点数组（vao）连接了顶点属性数据数组（行196）和顶点缓冲区（192）
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


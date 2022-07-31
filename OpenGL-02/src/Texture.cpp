#include "Texture.h"
#include "Renderer.h"
#include "stb_image/stb_image.h"

Texture::Texture(const string &path)
	:m_RendererID(0), m_FilePath(path) ,m_LocalBuffer(nullptr),
	m_Width(0) , m_Height(0),m_BPP(0)
{
	//OGL�����������½�
	//������ͼƬ�Ǵ����Ͻǿ�ʼɨ���
	//�����Ҫ���º��������Ƚ�������ֱ��ת
	//Ȼ���ٽ���OGL�ӵߵ�����������½�ɨ��
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);//4��ʾRGBA�ĸ��ܵ�

	//��������,����һ��������󣬲�����������ȾID
	GLCall(glGenTextures(1, &m_RendererID));
	//������
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//����ͼ��
	//��С������
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//��С������(���ش�ʵ������С)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//У���ı�����(ST������ͼ���е�XY)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	//��һ������ȡ�����ݸ�OGL
	//Ŀǰ��������λ���ڲ��ͱ��ػ�����
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	//���
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	//�������Ƿ������ݣ����ػ�����������Ϊ��Ա��������˷�����ڵ�����ʹ��
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	//��GPU��ɾ������
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	//ָ��һ�������,����32������ۣ�ÿ�μ���������Ҫ�趨�۵�λ��
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));//GL_TEXTURE0ʵ�ʶ��������ڼ���
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


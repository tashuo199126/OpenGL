#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	string m_FilePath;
	unsigned char *m_LocalBuffer;//����ı��ش洢ָ��
	int m_Width, m_Height, m_BPP;//���صĿ�͸��Լ�����λ��
public:
	Texture(const string &path);
	~Texture();

	void Bind(unsigned int slot = 0) const;//����slot���ڿ�ѡ����,��ָ���ദ����
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeignt() const { return m_Height; }
};
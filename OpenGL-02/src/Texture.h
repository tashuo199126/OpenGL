#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	string m_FilePath;
	unsigned char *m_LocalBuffer;//纹理的本地存储指针
	int m_Width, m_Height, m_BPP;//像素的宽和搞以及像素位数
public:
	Texture(const string &path);
	~Texture();

	void Bind(unsigned int slot = 0) const;//其中slot属于可选参数,可指定多处纹理
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeignt() const { return m_Height; }
};
#include "Texture.h"
#include "Renderer.h"
#include "stb_image/stb_image.h"

Texture::Texture(const string &path)
	:m_RendererID(0), m_FilePath(path) ,m_LocalBuffer(nullptr),
	m_Width(0) , m_Height(0),m_BPP(0)
{
	//OGL的中心在左下角
	//但正常图片是从左上角开始扫描的
	//因此需要以下函数帮助先将纹理将垂直翻转
	//然后再交由OGL从颠倒后的纹理左下角扫描
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);//4表示RGBA四个管道

	//加载纹理,生成一个纹理对象，并将其分配给渲染ID
	GLCall(glGenTextures(1, &m_RendererID));
	//绑定纹理
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//加载图像
	//缩小过滤器
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	//缩小过滤器(像素大，实际纹理小)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	//校正文本参数(ST类似于图形中的XY)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	//下一步将读取的数据给OGL
	//目前像素数据位于内部和本地缓冲区
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	//解绑
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	//缓冲区是否有数据，本地缓冲区被设置为成员变量，因此方便后期的其他使用
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	//从GPU上删除数据
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	//指定一个纹理槽,共有32个纹理槽，每次加载纹理需要设定槽的位置
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));//GL_TEXTURE0实际定义类似于计数
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}


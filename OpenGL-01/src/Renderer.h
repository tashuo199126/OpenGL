#pragma once
#include<GL/glew.h> 
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define ASSERT(x) if(!(x))  __debugbreak()//帮助差错打断点，__代表编译器内在函数
//定义宏清除之前的错误（最后不加;，因为GLCall会自己加）
#define GLCall(x)  GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__))//#x将x转换为字符串

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
	void Clear()const;
	void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;

};
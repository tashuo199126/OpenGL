#shader vertex
#version 330 core

layout(location = 0) in vec4 posion;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

//创建一个uniform将矩阵从CPU接收到着色器中
//投影矩阵（不属于视图和模型矩阵）
uniform mat4 u_MVP;

void main()
{
 gl_Position = u_MVP * posion;
 v_TexCoord = texCoord;
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	//需要知道纹理中的精确位置
	vec4 texColor = texture(u_Texture, v_TexCoord);
 //color = vec4(0.2,0.3,0.5,1.0);
	color = texColor;
}; 
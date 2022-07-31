#shader vertex
#version 330 core

layout(location = 0) in vec4 posion;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

//����һ��uniform�������CPU���յ���ɫ����
//ͶӰ���󣨲�������ͼ��ģ�;���
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
	//��Ҫ֪�������еľ�ȷλ��
	vec4 texColor = texture(u_Texture, v_TexCoord);
 //color = vec4(0.2,0.3,0.5,1.0);
	color = texColor;
}; 
#shader vertex
#version 330 core

layout(location = 0) in vec4 posion;

void main()
{
 gl_Position = posion;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
 //color = vec4(0.2,0.3,0.5,1.0);
	color = u_Color;
}; 
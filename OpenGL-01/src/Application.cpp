#include<iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

using namespace std;

#include"Renderer.h"
#include "IndexBuffer.h"
#include "VerTextBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//OpenGL���汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);//OpenGL�ΰ汾
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);//COMPAT
	//�ڶ�������Ϊʲô����GLFW_OPENGL_CORE_PROFILE,�����û�а󶨶����������
	//����OpenGL�����ļ���ʹVao����0��ΪĬ�ϵ��͡����ĵ�opengl�����ļ�ʹvao������ȫ����һ������
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{ 
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current 
	   OpenGL��ʼ��Ⱦ*/
	
	glfwMakeContextCurrent(window);
	//glewInit(); //glewInit()�������ĵ�ַ�������Щָ�롣
	glfwSwapInterval(1);//��ɫ�任�ٶ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		printf("Error\
");

	cout << glGetString(GL_VERSION) << endl;

	{
		//���㻺����
		float positions[] = {
			-0.5f, -0.5f,
			0.5f, -0.5f,
			0.5f, 0.5f,
			-0.5f, 0.5f,
		};

		//����������
		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};

		//�����������ID��
		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));
		
		VertexBufferLayout layout;
		layout.Push<float>(2);//������������������
		va.AddBuffer(vb, layout);
		

		IndexBuffer ib(indices, 6);
		
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
			//�������鶨��
		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.005f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			shader.Bind();
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f); 

			//��Ⱦ������
			renderer.Draw(va, ib, shader);

			if (r > 1.0f)
				increment = -0.005f;
			else if (r < 0.0f)
				increment = 0.005f;
			r += increment;
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}
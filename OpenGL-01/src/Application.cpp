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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//OpenGL主版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);//OpenGL次版本
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);//COMPAT
	//第二个参数为什么不是GLFW_OPENGL_CORE_PROFILE,简单理解没有绑定顶点数组对象
	//兼容OpenGL配置文件，使Vao对象0成为默认的油。核心的opengl配置文件使vao对象完全不是一个对象。
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{ 
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current 
	   OpenGL开始渲染*/
	
	glfwMakeContextCurrent(window);
	//glewInit(); //glewInit()将函数的地址分配给那些指针。
	glfwSwapInterval(1);//颜色变换速度
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		printf("Error\
");

	cout << glGetString(GL_VERSION) << endl;

	{
		//顶点缓冲区
		float positions[] = {
			-0.5f, -0.5f,
			0.5f, -0.5f,
			0.5f, 0.5f,
			-0.5f, 0.5f,
		};

		//索引缓冲区
		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};

		//顶点数组对象（ID）
		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));
		
		VertexBufferLayout layout;
		layout.Push<float>(2);//例子中用两个浮点数
		va.AddBuffer(vb, layout);
		

		IndexBuffer ib(indices, 6);
		
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
			//顶点数组定义
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

			//渲染器绘制
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
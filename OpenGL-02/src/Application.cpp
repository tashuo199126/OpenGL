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
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

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
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
			-50.0f, -50.0f, 0.0f, 0.0f,
			50.0f, -50.0f,1.0f, 0.0f,
			50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f, 50.0f,0.0f, 1.0f,
		};

		//索引缓冲区
		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};

		//启用混合
		GLCall(glEnable(GL_BLEND));
		//设置打开混合功能
		//gl如何正确混合alpha像素（1-源码alpha=目标alpha）
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//顶点数组对象（ID）
		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		VertexArray va;
		VertexBuffer vb(positions, 4 *4 * sizeof(float));//顶点坐标行*列
		
		VertexBufferLayout layout;
		layout.Push<float>(2);//例子中用两个浮点数
		layout.Push<float>(2);//添加纹理坐标
		va.AddBuffer(vb, layout);
		

		IndexBuffer ib(indices, 6);
		
		//投影矩阵
		//将矩形窗口(4:3)调用glm库改为正方形
		//-2.0f, 2.0f, -1.5f, 1.5f（左右下上）最后两个数指定文件（这里没指定）
		//将顶点简单的投影到二维画面上
		//让着色器使用前面定义的顶点数组和下面的矩阵相乘
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, 1.0f, -1.0f);//正交矩阵(XY之间有正交投影)
		//视图矩阵
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		////模型矩阵
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0));
		//
		////投影矩阵*视图矩阵*模型矩阵
		//glm::mat4 mvp = proj * view * model;

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		//矩阵投影
		//shader.SetUniformMat4f("u_MVP", mvp);
		
		Texture texture("res/textures/2.png");
		texture.Bind();
		//通过uniforms读取纹理槽
		//想着色器发送整数（纹理槽的标号，也称为采样器）
		//上面绑定几，下面uniform就要跟第几号纹理槽
		shader.SetUniform1i("u_Texture", 0);
		//顶点数组定义
		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		Renderer renderer;

		ImGui::CreateContext();
		//初始化它
		ImGui_ImplGlfwGL3_Init(window, true);
		//样式设置为暗
		ImGui::StyleColorsDark();

		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);


		float r = 0.0f;
		float increment = 0.005f;
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			//imgui设置一个新的框架
			ImGui_ImplGlfwGL3_NewFrame();

			//第一个图形
			{
				//模型矩阵
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				//投影矩阵*视图矩阵*模型矩阵
				glm::mat4 mvp = proj * view * model;
				shader.Bind();

				shader.SetUniformMat4f("u_MVP", mvp);//该位置实现在每一帧重新计算模型矩阵和MVP
					//渲染器绘制
				renderer.Draw(va, ib, shader);
			}

			//第二个图形
			{
				//模型矩阵
				glm::mat4 modelB = glm::translate(glm::mat4(1.0f), translationB);
				//投影矩阵*视图矩阵*模型矩阵
				glm::mat4 mvpB = proj * view * modelB;
				shader.Bind();

				//绘制第二个图像
				shader.SetUniformMat4f("u_MVP", mvpB);//该位置实现在每一帧重新计算模型矩阵和MVP
				renderer.Draw(va, ib, shader);
			}


			if (r > 1.0f)
				increment = -0.005f;
			else if (r < 0.0f)
				increment = 0.005f;
			r += increment;

			bool show_demo_window = true;
			bool show_another_window = false;
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



			// 1. Show a simple window.
// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
			{
				                          // Display some text (you can use a format string too)
				ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f,960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			
			}




			//绘制东西
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
 
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	//ingui清除关闭
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
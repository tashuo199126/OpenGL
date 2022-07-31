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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//OpenGL���汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);//OpenGL�ΰ汾
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);//COMPAT
	//�ڶ�������Ϊʲô����GLFW_OPENGL_CORE_PROFILE,�����û�а󶨶����������
	//����OpenGL�����ļ���ʹVao����0��ΪĬ�ϵ��͡����ĵ�opengl�����ļ�ʹvao������ȫ����һ������
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
			-50.0f, -50.0f, 0.0f, 0.0f,
			50.0f, -50.0f,1.0f, 0.0f,
			50.0f,  50.0f, 1.0f, 1.0f,
			-50.0f, 50.0f,0.0f, 1.0f,
		};

		//����������
		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};

		//���û��
		GLCall(glEnable(GL_BLEND));
		//���ô򿪻�Ϲ���
		//gl�����ȷ���alpha���أ�1-Դ��alpha=Ŀ��alpha��
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//�����������ID��
		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		VertexArray va;
		VertexBuffer vb(positions, 4 *4 * sizeof(float));//����������*��
		
		VertexBufferLayout layout;
		layout.Push<float>(2);//������������������
		layout.Push<float>(2);//�����������
		va.AddBuffer(vb, layout);
		

		IndexBuffer ib(indices, 6);
		
		//ͶӰ����
		//�����δ���(4:3)����glm���Ϊ������
		//-2.0f, 2.0f, -1.5f, 1.5f���������ϣ����������ָ���ļ�������ûָ����
		//������򵥵�ͶӰ����ά������
		//����ɫ��ʹ��ǰ�涨��Ķ������������ľ������
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, 1.0f, -1.0f);//��������(XY֮��������ͶӰ)
		//��ͼ����
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		////ģ�;���
		//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0));
		//
		////ͶӰ����*��ͼ����*ģ�;���
		//glm::mat4 mvp = proj * view * model;

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
		//����ͶӰ
		//shader.SetUniformMat4f("u_MVP", mvp);
		
		Texture texture("res/textures/2.png");
		texture.Bind();
		//ͨ��uniforms��ȡ�����
		//����ɫ����������������۵ı�ţ�Ҳ��Ϊ��������
		//����󶨼�������uniform��Ҫ���ڼ��������
		shader.SetUniform1i("u_Texture", 0);
		//�������鶨��
		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		Renderer renderer;

		ImGui::CreateContext();
		//��ʼ����
		ImGui_ImplGlfwGL3_Init(window, true);
		//��ʽ����Ϊ��
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

			//imgui����һ���µĿ��
			ImGui_ImplGlfwGL3_NewFrame();

			//��һ��ͼ��
			{
				//ģ�;���
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				//ͶӰ����*��ͼ����*ģ�;���
				glm::mat4 mvp = proj * view * model;
				shader.Bind();

				shader.SetUniformMat4f("u_MVP", mvp);//��λ��ʵ����ÿһ֡���¼���ģ�;����MVP
					//��Ⱦ������
				renderer.Draw(va, ib, shader);
			}

			//�ڶ���ͼ��
			{
				//ģ�;���
				glm::mat4 modelB = glm::translate(glm::mat4(1.0f), translationB);
				//ͶӰ����*��ͼ����*ģ�;���
				glm::mat4 mvpB = proj * view * modelB;
				shader.Bind();

				//���Ƶڶ���ͼ��
				shader.SetUniformMat4f("u_MVP", mvpB);//��λ��ʵ����ÿһ֡���¼���ģ�;����MVP
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




			//���ƶ���
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
 
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	//ingui����ر�
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}
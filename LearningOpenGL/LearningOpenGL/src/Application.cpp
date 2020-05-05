#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <windows.h>

#include "util.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

float greenVal = 0.0f;
float greenValInc = 0.05f;

void UpdateGreenValue(Shader& shader)
{
	shader.SetUniform4f("u_Color", 0.6f, greenVal, 0.9f, 1.0f);
	if ((greenVal < 0.0f) || (greenVal > 1.0f))
		greenValInc *= -1;
	greenVal += greenValInc;
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	SwitchToCoreProfile();

	window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		return -1;

	EnableGLDebugCallback();
	PrintOpenGlVersionInfo();

	{
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, // 0
			 0.5f, -0.5f, 1.0f, 0.0f, // 1
			 0.5f,  0.5f, 1.0f, 1.0f, // 2
			-0.5f,  0.5f, 0.0f, 1.0f  // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		VertexArray va;
		VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBufferAndLayout(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
		shader.SetUniform4f("u_Color", 0.6f, 1.0f, 0.9f, 1.0f);

		Texture texture("res/textures/winniethepooh.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0); // Bind texture to slot 0

		/* Unbind everything */
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		while (!glfwWindowShouldClose(window))
		{
			renderer.Clear();

			shader.Bind();
			UpdateGreenValue(shader);
			renderer.Draw(va, ib, shader);
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

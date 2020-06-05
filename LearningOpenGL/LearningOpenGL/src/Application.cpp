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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int windowWidth = 960;
int windowHeight = 540;

int squareSideLength = 500;

float radius = 0.0f;
float radiusInc = 2.0f;

void UpdateRadius(Shader& shader)
{
	shader.SetUniform1f("u_Radius", radius);
	if ((radius < 0.0f) || (radius > (squareSideLength / 2)))
		radiusInc *= -1;
	radius += radiusInc;
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	SwitchToCoreProfile();

	window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
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
		float offsetWidth = 0.0f;
		float offsetHeight = 0.0f;
		float vertices[] = {
			offsetWidth, offsetHeight, 0.0f, 0.0f,
			offsetWidth + (float)squareSideLength, offsetHeight, 1.0f, 0.0f,
			offsetWidth + (float)squareSideLength, offsetHeight + (float)squareSideLength, 1.0f, 1.0f,
			offsetWidth, offsetHeight + (float)squareSideLength, 0.0f, 1.0f
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

		/* Create projection matrix */
		glm::mat4 proj = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		glm::mat4 mvp = proj * view * model;

		Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
		shader.SetUniform4f("u_Color", 0.6f, 1.0f, 0.9f, 1.0f);
		shader.SetUniform4f("u_Center", ((float)squareSideLength + offsetWidth) / 2, ((float)squareSideLength + offsetHeight) / 2, 0.0f, 1.0f);
		shader.SetUniformMat4f("u_MVP", mvp);

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
			UpdateRadius(shader);
			renderer.Draw(va, ib, shader);
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

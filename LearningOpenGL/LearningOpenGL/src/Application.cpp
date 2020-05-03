#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <windows.h>

#include "util.h"
#include "ShaderLoader.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#define ASSERT(x) if (!(x)) __debugbreak();

GLint u_Color = -1;
float greenVal = 0.0f;
float greenValInc = 0.05f;

GLuint vao;
GLuint shader;
VertexBuffer* vb;
IndexBuffer* ib;

static void Init(std::unique_ptr<VertexBuffer>& vb, std::unique_ptr<IndexBuffer>& ib)
{
	float vertices[] = {
		-0.5f, -0.5f, // 0
		 0.5f, -0.5f, // 1
		 0.5f,  0.5f, // 2
		-0.5f,  0.5f, // 3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	/* Initialize vertex array object */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vb = std::make_unique<VertexBuffer>(vertices, 4 * 2 * sizeof(float));

	/* Link vertex buffer to vao */
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	glEnableVertexAttribArray(0);

	ib = std::make_unique<IndexBuffer>(indices, 6);

	shader = LoadShader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	glUseProgram(shader);

	u_Color = glGetUniformLocation(shader, "u_Color");
	glUniform4f(u_Color, 0.6f, 1.0f, 0.9f, 1.0f);

	/* Unbind everything */
	glBindVertexArray(0);
	glUseProgram(0);
	
	vb->Unbind();
	ib->Unbind();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void UpdateGreenValue()
{
	glUniform4f(u_Color, 0.6f, greenVal, 0.9f, 1.0f);
	if ((greenVal < 0.0f) || (greenVal > 1.0f))
		greenValInc *= -1;
	greenVal += greenValInc;
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	/* Create an OpenGL context with the CORE_PROFILE */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set OpenGL profile to be core

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
		std::unique_ptr<VertexBuffer> vb;
		std::unique_ptr<IndexBuffer> ib;

		Init(vb, ib);

		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shader);
			glBindVertexArray(vao);
			ib->Bind();

			UpdateGreenValue();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

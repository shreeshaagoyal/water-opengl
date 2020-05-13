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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int windowWidth = 960;
int windowHeight = 540;

int squareSideLength = 200;

float greenVal = 0.0f;
float greenValInc = 0.05f;
int camera_x = 0;
int cameraDelta_x = -10;
int cameraDeltaReset_x = windowWidth + squareSideLength;
glm::mat4 mvp;

void UpdateGreenValue(Shader& shader)
{
	shader.SetUniform4f("u_Color", 0.6f, greenVal, 0.9f, 1.0f);
	if ((greenVal < 0.0f) || (greenVal > 1.0f))
		greenValInc *= -1;
	greenVal += greenValInc;
}

void UpdateRotationMatrix(Shader& shader)
{
	int cameraTranslation;
	if (camera_x < ((windowWidth + squareSideLength) / std::abs(cameraDelta_x)))
	{
		cameraTranslation = cameraDelta_x;
		camera_x++;
	}
	else
	{
		cameraTranslation = cameraDeltaReset_x;
		camera_x = 0;
	}
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1 * cameraTranslation, 0, 0));
	mvp *= view;
	shader.SetUniformMat4f("u_MVP", mvp);
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
		float height = 100.0f;
		float vertices[] = {
			(float)(-1 * squareSideLength), height, 0.0f, 0.0f,
			0.0f, height, 1.0f, 0.0f,
			0.0f, height + (float)squareSideLength, 1.0f, 1.0f,
			(float)(-1 * squareSideLength), height + (float)squareSideLength, 0.0f, 1.0f
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
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 10, 0));

		mvp = proj * model;

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
			UpdateRotationMatrix(shader);
			UpdateGreenValue(shader);
			renderer.Draw(va, ib, shader);
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

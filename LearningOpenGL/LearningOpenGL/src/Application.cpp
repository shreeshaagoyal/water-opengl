#include "stdafx.h"

int windowWidth = 960;
int windowHeight = 540;

float defaultThickness = 2.0f;
float defaultRadiusInc = 2.0f;

std::set<std::unique_ptr<Ripple>> ripples;

auto lastRippleTime = std::chrono::steady_clock::now();
void insertRipple(double xpos, double ypos)
{
	auto now = std::chrono::steady_clock::now();
	auto msSinceLastRipple = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRippleTime).count();
	if (msSinceLastRipple > 100)
	{
		ripples.insert(std::make_unique<Ripple>(xpos, ypos, 0.0f, defaultRadiusInc, 140.0f, defaultThickness));
		lastRippleTime = now;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		insertRipple(xpos, ypos);
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		return;
	else
		insertRipple(xpos, ypos);
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
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);

	if (glewInit() != GLEW_OK)
		return -1;

	EnableGLDebugCallback();
	PrintOpenGlVersionInfo();

	{
		float offsetWidth = 0.0f;
		float offsetHeight = 0.0f;
		int maxRadius = std::max(windowWidth, windowHeight);
		float vertices[] = {
			0.0f,		0.0f,		0.0f, 0.0f,
			maxRadius,  0.0f,		1.0f, 0.0f,
			maxRadius,  maxRadius,	1.0f, 1.0f,
			0.0f,		maxRadius,	0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		VertexArray va;
		VertexBuffer vb(vertices, 4 * 4 * sizeof(float));

		// First 2 floats are position. Second 2 floats are texture coords.
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBufferAndLayout(vb, layout);

		IndexBuffer ib(indices, 6);

		/* Create projection matrix */
		glm::mat4 proj = glm::ortho(0.0f, (float)windowWidth, 0.0f, (float)windowHeight, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

		Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
		shader.SetUniform4f("u_Color", 0.6f, 1.0f, 0.9f, 1.0f);

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

			{
				std::vector<std::unique_ptr<Ripple>*> ripplesToDelete;
				for (const auto& ripple : ripples)
					if (ripple->deletePending)
						ripplesToDelete.push_back(const_cast<std::unique_ptr<Ripple>*>(&ripple));

				for (const auto& ripple : ripplesToDelete)
					ripples.erase(*ripple);
			}

			for (const auto& ripple : ripples)
			{
				glm::vec3 translation
				{
					ripple->xpos - ripple->maxRadius,
					windowHeight - ripple->ypos - ripple->maxRadius,
					0.0f
				};
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
				glm::mat4 mvp = proj * view * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				shader.SetUniform1f("u_Thickness", ripple->thickness);
				shader.SetUniform4f
				(
					"u_Center",
					offsetWidth + translation.x + ripple->maxRadius,
					offsetHeight + translation.y + ripple->maxRadius,
					0.0f,
					1.0f
				);
				ripple->UpdateRadius(shader);
				renderer.Draw(va, ib, shader);
			}
			
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

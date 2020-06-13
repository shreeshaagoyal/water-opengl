#include "stdafx.h"

int windowWidth = 960;
int windowHeight = 540;

float defaultRadiusInc = 2.0f;

std::set<std::unique_ptr<Ripple>> ripplePool;

auto lastRippleTime = std::chrono::steady_clock::now();
void insertRipple(float xpos, float ypos)
{
	auto now = std::chrono::steady_clock::now();
	auto msSinceLastRipple = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRippleTime).count();
	if (msSinceLastRipple > 100)
	{
		ripplePool.insert(std::make_unique<Ripple>(xpos, ypos, 0.0f, defaultRadiusInc, 140.0f));
		lastRippleTime = now;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "[" << xpos << "," << ypos << "]" << std::endl;
		insertRipple(static_cast<float>(xpos), static_cast<float>(windowHeight - ypos));
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		return;
	else
		insertRipple(static_cast<float>(xpos), static_cast<float>(windowHeight - ypos));
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
	EnableBasicBlending();

	{
		float offsetWidth = 0.0f;
		float offsetHeight = 0.0f;
		int maxRadius = std::max(windowWidth, windowHeight);
		maxRadius = 30.0f;

		const size_t upperLimit = 1000;

		VertexBuffer vb(nullptr, upperLimit * 4 * sizeof(Ripple::vertex), true);
		IndexBuffer ib(nullptr, 6 * upperLimit, true);

		/* Layout:
		| Position coords [2 floats]
		| Center coords [2 floats]
		| Current radius [1 float]
		*/
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<float>(1);

		Shader shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
		shader.SetUniform4f("u_Color", 0.6f, 1.0f, 0.9f, 1.0f);

		VertexArray va;
		Renderer renderer;

		va.Unbind();
		vb.Unbind();
		ib.Unbind();

		shader.Bind();

		while (!glfwWindowShouldClose(window))
		{
			{
				std::vector<std::unique_ptr<Ripple>*> ripplesToDelete;
				for (const auto& ripple : ripplePool)
				{
					if (ripple->deletePending)
						ripplesToDelete.push_back(const_cast<std::unique_ptr<Ripple>*>(&ripple));
				}

				for (const auto& ripple : ripplesToDelete)
					ripplePool.erase(*ripple);
			}

			float vertices[upperLimit * sizeof(Ripple::vertex)];
			size_t index = 0;
			for (const auto &ripple : ripplePool)
			{
				auto positions = ripple->GetAsArray();
				std::memcpy(vertices + (index * positions.size()), positions.data(), positions.size() * sizeof(float));
				index++;
			}

			std::array<unsigned int, 6> indice = { 0, 1, 2, 2, 3, 0 };
			unsigned int indices[upperLimit * 6];
			std::memcpy(indices, indice.data(), indice.size() * sizeof(unsigned int));
			for (size_t i = 1; i < ripplePool.size(); ++i)
			{
				for (size_t j = 0; j < indice.size(); ++j)
				{
					indice[j] = indice[j] + 4;
				}
				std::memcpy(indices + (i * indice.size()), indice.data(), indice.size() * sizeof(unsigned int));
			}

			{
				glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(windowWidth), 0.0f, static_cast<float>(windowHeight), -1.0f, 1.0f);
				glm::mat4 mvp = proj;
				shader.SetUniformMat4f("u_MVP", mvp);
			}

			vb.Populate(sizeof(vertices), vertices);
			va.AddBufferAndLayout(vb, layout);
			ib.Populate(sizeof(indices), indices);

			for (const auto &ripple : ripplePool)
				ripple->UpdateRadius();

			std::memset(vertices, 0, sizeof(vertices));
			renderer.Clear();
			renderer.Draw(va, ib, shader);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}

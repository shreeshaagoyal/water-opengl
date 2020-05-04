#include "util.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>

void Crash()
{
	*(reinterpret_cast<char*>(0)) = 0;
}

static void GLAPIENTRY GLDebugCallback
(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam
)
{
	bool crashProgram = false;

	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
	{
		std::cout << "[OpenGL Notification]: ";
	}
	else
	{
		std::cout << "[OpenGL Warning/Error]: ";
		crashProgram = true;
	}

	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 4);
	std::cout << message << std::endl;
	SetConsoleTextAttribute(hConsole, 15);

	if (crashProgram)
		Crash();
}

void EnableGLDebugCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLDebugCallback, nullptr);
}

void PrintOpenGlVersionInfo()
{
	auto printVersion = [](uint32_t version) {
		auto str = (const char*) glGetString(version);
		std::cout << (str ? str : "Error") << std::endl;
	};

	printVersion(GL_VERSION);
	printVersion(GL_VENDOR);
	printVersion(GL_RENDERER);
}

/* Create an OpenGL context with the CORE_PROFILE */
void SwitchToCoreProfile()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set OpenGL profile to be core
}

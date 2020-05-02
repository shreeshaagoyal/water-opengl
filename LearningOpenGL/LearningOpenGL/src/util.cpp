#include "util.h"
#include <GL/glew.h>
#include <windows.h>
#include <iostream>

void Crash()
{
	*(reinterpret_cast<char*>(0)) = 0;
}

typedef void (GLAPIENTRY* DEBUGPROC)
(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam
);

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

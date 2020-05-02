#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "ShaderLoader.h"

// TODO: make a function to read in any file into a string var
static std::string ReadFile(const std::string& filepath)
{
	std::fstream stream(filepath);

	std::string line;
	std::stringstream ss;

	while (getline(stream, line))
	{
		ss << line << '\n';
	}

	return ss.str();
}

static GLuint CompileShader(unsigned int shaderType, const std::string& source)
{
	GLuint id = glCreateShader(shaderType);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	GLint result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	// Shader did not compile successfully
	if (result == GL_FALSE)
	{
		// Query the error message length
		GLint length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

GLuint LoadShader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
{
	const std::string vertexSource = ReadFile(vertexFilepath);
	const std::string fragmentSource = ReadFile(fragmentFilepath);

	return CreateShader(vertexSource, fragmentSource);
}

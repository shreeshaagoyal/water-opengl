#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "util.h"

Shader::Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath)
	: m_VertexFilepath(vertexFilepath), m_FragmentFilepath(fragmentFilepath), m_RendererID(0)
{
	m_RendererID = LoadShader();
	Bind();
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int v)
{
	glUniform1i(GetUniformLocation(name), v);
}

void Shader::SetUniform1f(const std::string& name, float v)
{
	glUniform1f(GetUniformLocation(name), v);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		std::cout << "Uniform variable '" << name << "' doesn't exist!" << std::endl;
	
	m_UniformLocationCache[name] = location;
	return location;
}

std::string Shader::ReadFile(const std::string& filepath)
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

GLuint Shader::CompileShader(unsigned int shaderType, const std::string& source)
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
		ASSERT(false);
	}

	return id;
}

GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

GLuint Shader::LoadShader()
{
	const std::string vertexSource = ReadFile(m_VertexFilepath);
	const std::string fragmentSource = ReadFile(m_FragmentFilepath);

	return CreateShader(vertexSource, fragmentSource);
}

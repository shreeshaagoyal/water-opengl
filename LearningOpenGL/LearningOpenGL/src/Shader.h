#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

struct Shader
{
private:
	GLuint m_RendererID;
	const std::string& m_VertexFilepath;
	const std::string& m_FragmentFilepath;

	GLuint LoadShader();
	static std::string ReadFile(const std::string& filepath);
	GLuint CompileShader(unsigned int shaderType, const std::string& source);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	GLint GetUniformLocation(const std::string& name) const;
	mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
public:
	Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
	~Shader();
	void Bind() const;
	void Unbind() const;
	void SetUniform1i(const std::string& name, int v);
	void SetUniform1f(const std::string& name, float v);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
};

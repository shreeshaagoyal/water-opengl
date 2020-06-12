#pragma once

#include <GL/glew.h>

class VertexBuffer
{
private:
	GLuint m_RendererID;
public:
	VertexBuffer(const void* data, GLuint size, bool isDynamic = false);
	~VertexBuffer();

	void Populate(GLsizeiptr size, const GLvoid *data) const;
	void Bind() const;
	void Unbind() const;
};
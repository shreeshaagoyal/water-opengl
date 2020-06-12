#pragma once

#include <GL/glew.h>

class IndexBuffer
{
private:
	GLuint m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count, bool isDynamic = false);
	~IndexBuffer();

	void Populate(GLsizeiptr size, const GLvoid *data) const;
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
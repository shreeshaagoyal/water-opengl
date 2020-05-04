#pragma once

#include <vector>
#include <GL/glew.h>

#include "Renderer.h"

struct BufferLayoutElement
{
	GLenum type;
	GLint count;
	GLboolean normalized;

	static GLsizei GetSizeOfType(GLenum type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<BufferLayoutElement> m_Elements;
	GLsizei m_Stride;
public:
	VertexBufferLayout() : m_Stride(0) {}
	template<typename T> void Push(GLint count);
	template<> void Push<GLfloat>(GLint count);
	template<> void Push<GLuint>(GLint count);
	template<> void Push<GLubyte>(GLint count);
	inline const std::vector<BufferLayoutElement>& GetElements() const { return m_Elements; }
	inline GLsizei GetStride() const { return m_Stride; }
};
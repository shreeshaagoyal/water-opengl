#pragma once

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	GLuint m_RendererID;
public:
	VertexArray();
	~VertexArray();
	void AddBufferAndLayout(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void Bind() const;
	void Unbind() const;
};
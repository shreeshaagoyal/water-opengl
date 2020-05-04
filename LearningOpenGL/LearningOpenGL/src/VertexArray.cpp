#include "VertexArray.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddBufferAndLayout(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (GLuint i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		glVertexAttribPointer
		(
			i,
			element.count,
			element.type,
			element.normalized,
			layout.GetStride(),
			(const GLvoid*)offset
		);
		glEnableVertexAttribArray(i);
		offset += element.count * BufferLayoutElement::GetSizeOfType(element.type);
	}
}

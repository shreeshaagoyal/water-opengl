#include "Renderer.h"
#include "util.h"

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	va.Bind();
	ib.Bind();
	shader.Bind();
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

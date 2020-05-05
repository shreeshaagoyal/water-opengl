#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

struct Renderer
{
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

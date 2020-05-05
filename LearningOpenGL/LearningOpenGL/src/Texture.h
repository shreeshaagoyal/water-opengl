#pragma once

#include <GL/glew.h>
#include <string>

class Texture
{
private:
	GLuint m_RendererID;
	std::string m_Filepath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BytesPerPixel;
public:
	Texture(const std::string& filepath);
	~Texture();
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;
	inline int GetWidth() { return m_Width; }
	inline int GetHeight() { return m_Height; }
};

#pragma once

#include <string>
#include "glew/include/GL/glew.h"

class Texture {
private:

	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	// the unique ID of the buffer
	unsigned int m_RendererID;

	Texture(const std::string& path);
	~Texture();

	inline int GetHeight() const {
		return m_Height;
	}
	inline int GetWidth() const {
		return m_Width;
	}
};
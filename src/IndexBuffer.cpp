#include "IndexBuffer.hpp"
#include "Debugging.hpp"
#include "glew/include/GL/glew.h"

// create the index buffer and fill
IndexBuffer::IndexBuffer(const unsigned int *data, int count)
	: m_Count(count) {
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}
